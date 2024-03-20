//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_STOTALCOMPANY_H
#define CITYOFWEIRDFISHES_STOTALCOMPANY_H

#include <vector>
#include <memory>
#include "sCommon.h"
#include "../grids/gIGrid.h"
#include "../../behaviour/company/sCompanyCompiler.h"
#include "../../behaviour/company/code/sCodeStoratge.h"

class rVectorCompanies {
public:

    rVectorCompanies(uint32_t nMaxCompanies) {
        tVecComp = std::vector<std::pair<uint32_t, std::shared_ptr<objCompany>>>(nMaxCompanies, {-1, nullptr});
        for (int i = 0; i < nMaxCompanies - 1; i++) {
            tVecComp[i] = {i + 1, nullptr};
        }
        tVecComp[tVecComp.size() - 1] = {-1, nullptr};
        fEmpty = 0;
    }

    std::shared_ptr<objCompany> getDestByComp(uint32_t rCar) {
        return tVecComp[rCar].second;
    }

    uint32_t
    addComp(const std::list<std::pair<int, int>> &sTilesStart, const objCompany::objComp_activeDates &cActiveDates) {
        uint32_t prevFEmpty = fEmpty;
        fEmpty = tVecComp[fEmpty].first;
        tVecComp[prevFEmpty] = {0,
                                std::make_shared<objCompany>(
                                        prevFEmpty, sTilesStart,
                                        cActiveDates)};/*
                                        objCompany::objComp_activeDates(
                                                {true, true, true, true, true, false, false},
                                                {0, 122}))}; //TODO REAL DAILY*/
        return prevFEmpty;
    }

    void removeComp(uint32_t rCar) {
        tVecComp[rCar] = {fEmpty, {}};
        fEmpty = rCar;
    }

    std::vector<sCompanyCompiler::sCCIntentions> getTotalIntentions(const std::shared_ptr<sCodeStorage> &sSCode) {
        std::vector<sCompanyCompiler::sCCIntentions> ret;
        for (auto &i: tVecComp) {
            if (i.second != nullptr) {
                std::vector<sCompanyCompiler::sCCIntentions> rComp =
                        sCompanyCompiler::givenCode(sSCode->getCodeByUuid(i.second->c_uuid).sCO_Code, i.second);
                ret.insert(ret.end(), rComp.begin(), rComp.end());
            }
        }
        return ret;
    }

    void applyEcoWeek() {
        for (auto &i: tVecComp)
            if (i.second != nullptr)
                i.second->c_cActiveFunds -= i.second->c_objFortnight;
    }

    void applyEcoMonth() {
        for (auto &i: tVecComp)
            if (i.second != nullptr)
                i.second->c_cActiveFunds -= i.second->c_objMonth;
    }

    void applyEcoYear() {
        for (auto &i: tVecComp)
            if (i.second != nullptr)
                i.second->c_cActiveFunds -= i.second->c_objYear;
    }

private:
    std::vector<std::pair<uint32_t, std::shared_ptr<objCompany>>> tVecComp;
    uint32_t fEmpty;
};

class sTotalCompany {
public:
    explicit sTotalCompany(uint32_t maxComp) : vTotalComp(maxComp) {}

    void addCompanyAtPosition(const std::shared_ptr<gIGrid<std::list<uint32_t>>> &gLayer,
                              const std::list<std::pair<int, int>> &vecNPos) {
        std::random_device rd;
        std::mt19937 gen(rd());
        // TODO BASSAT EN LA MATEIX LLAVOR;
        std::uniform_int_distribution<> disDay(0, (int) vActiveDaysValid.size() - 1);
        std::uniform_int_distribution<> disHour(0, (int) vActiveHoursValid.size() - 1);
        int randomIndexDay = disDay(gen);
        int randomIndexHour = disHour(gen);

        uint32_t idNewComp = vTotalComp.addComp(vecNPos,
                                                {vActiveDaysValid[randomIndexDay], vActiveHoursValid[randomIndexHour]});
        for (const auto &nPos: vecNPos) {
            auto p = gLayer->get({nPos.second, nPos.first});
            p.push_front(idNewComp); //AAAAAAAAAAAAAAAAAAAAAA
            gLayer->set({nPos.second, nPos.first}, p);//TODO no m'agrada que aixo vulgi dir que estem creant una copia.
        }
    }

    std::shared_ptr<objCompany> getCompanyByUUID(uint32_t index) {
        return vTotalComp.getDestByComp(index);
    }

    std::vector<objCompany> getVecCompByUUID(const std::list<uint32_t> &tList) {
        std::vector<objCompany> r;
        for (const uint32_t l: tList) {
            r.push_back(*getCompanyByUUID(l));
        }
        return r;
    }

    void applyEcoWeek() {
        vTotalComp.applyEcoWeek();
    }

    void applyEcoMonth() {
        vTotalComp.applyEcoMonth();
    }

    void applyEcoYear() {
        vTotalComp.applyEcoYear();
    }

    std::vector<sCompanyCompiler::sCCIntentions>
    getTotalIntentions(const std::shared_ptr<sCodeStorage> &sSCode) { return vTotalComp.getTotalIntentions(sSCode); }

private:
    rVectorCompanies vTotalComp;

    std::vector<std::vector<bool>> vActiveDaysValid = {{true,  true,  true,  true,  true, false, false},
                                                       {true,  true,  true,  true,  true, true,  false},
                                                       {false, false, false, false, true, true,  true}};
    std::vector<std::pair<uint32_t, uint32_t>> vActiveHoursValid = {{108, 204},
                                                                    {96,  192},
                                                                    {120, 216},
                                                                    {216, 276}};
};


#endif //CITYOFWEIRDFISHES_STOTALCOMPANY_H

//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYSTORAGE_H
#define CITYOFWEIRDFISHES_SCOMPANYSTORAGE_H

#include <vector>
#include <memory>
#include "../../structure/obj/sCommon.h"
#include "../../structure/grids/gIGrid.h"
#include "sCompanyCompiler.h"
#include "code/sCodeStoratge.h"

class rVectorCompanies {
public:

    explicit rVectorCompanies(uint32_t nMaxCompanies) {
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
    addComp(const std::list<std::pair<int, int>> &sTilesStart, uint8_t typeCompany,
            const objCompany::objComp_activeDates &cActiveDates) {
        uint32_t prevFEmpty = fEmpty;
        fEmpty = tVecComp[fEmpty].first;
        tVecComp[prevFEmpty] = {0,
                                std::make_shared<objCompany>(
                                        prevFEmpty, sTilesStart, typeCompany, cActiveDates)};
        tVecComp[prevFEmpty].second->c_cActiveFunds = 5000.0;
        return prevFEmpty;
    }

    void removeComp(uint32_t rCar) {
        tVecComp[rCar] = {fEmpty, {}};
        fEmpty = rCar;
    }

    std::vector<sCompanyCompiler::sCCIntentions>
    getTotalIntentions(const std::shared_ptr<sCodeStorage> &sSCode, uint32_t tTimer) {
        std::vector<sCompanyCompiler::sCCIntentions> ret;
        for (auto &i: tVecComp) {
            if (i.second != nullptr) {
                std::pair<uint32_t, uint32_t> pairTimeActive = i.second->c_activeDates.c_StrEndTime;
                if (tTimer >= pairTimeActive.first && tTimer <= pairTimeActive.second) {
                    std::vector<sCompanyCompiler::sCCIntentions> rComp =
                            sCompanyCompiler::givenCode(sSCode->getCodeByUuid(i.second->c_uuid).sCO_Code, i.second);
                    ret.insert(ret.end(), rComp.begin(), rComp.end());
                }
            }
        }
        return ret;
    }

    std::vector<std::shared_ptr<objCompany>>
    getTotalBankruptcy(uint32_t tTimer) {
        std::vector<std::shared_ptr<objCompany>> ret;
        for (auto &i: tVecComp)
            if (i.second != nullptr && i.second->c_cActiveFunds < 0 &&
                (i.second->c_objYear + i.second->c_objMonth * 12 + i.second->c_objFortnight * 36) < 0)
                ret.push_back(i.second);

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

    std::vector<std::pair<uint32_t, int>> getDiffEmployeesByLocation(uint32_t inRTimer) {
        std::vector<std::pair<uint32_t, int>> ret;
        for (auto &i: tVecComp) {
            if (i.second != nullptr) {
                std::pair<uint32_t, uint32_t> pairTimeActive = i.second->c_activeDates.c_StrEndTime;
                if (inRTimer >= pairTimeActive.first && inRTimer <= pairTimeActive.second &&
                    i.second->c_nEmployee != i.second->c_cActiveLocations.size() * 2)
                    ret.emplace_back(i.second->c_uuid, i.second->c_cActiveLocations.size() * 2 - i.second->c_nEmployee);
            }
        }
        return ret;
    }

private:
    std::vector<std::pair<uint32_t, std::shared_ptr<objCompany>>> tVecComp;
    uint32_t fEmpty;
};

class sCompanyStorage {
public:
    explicit sCompanyStorage(uint32_t inGridSize, uint32_t maxComp, const std::shared_ptr<sCodeStorage> &sStorageCode)
            : sCT_vTotalComp(maxComp), sCT_sCodeS(sStorageCode) {

        gLayerOwnership = std::make_shared<gBasicGrid<std::list<uint32_t>>>
                (gBasicGrid<std::list<uint32_t>>(inGridSize, inGridSize, {}));

        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            sCT_genRand.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            sCT_genRand.seed(static_cast<unsigned int>(time(nullptr)));
    }

    uint32_t createCompany(const std::list<std::pair<int, int>> &vecNPos, uint8_t typeCompany) {
        std::uniform_int_distribution<> disDay(0, (int) sCT_vActiveDaysValid.size() - 1);
        std::uniform_int_distribution<> disHour(0, (int) sCT_vActiveHoursValid.size() - 1);
        int randomIndexDay = disDay(sCT_genRand);
        int randomIndexHour = disHour(sCT_genRand);

        uint32_t _idNewComp = sCT_vTotalComp.addComp(vecNPos, typeCompany,
                                                     {sCT_vActiveDaysValid[randomIndexDay],
                                                      sCT_vActiveHoursValid[randomIndexHour]});
        sCT_sCodeS->initNewCode(_idNewComp);
        getCompanyByUUID(_idNewComp)->c_cCode = sCT_sCodeS->getPointerCodeByUuid(_idNewComp);
        for (const auto &nPos: vecNPos)
            addRefPosOwnCompany(nPos, _idNewComp);

        return _idNewComp;
    }

    bool isCompanyInPosition(const std::pair<int,int>& inPCell){return !gLayerOwnership->get(inPCell).empty();}

    void addRefPosOwnCompany(std::pair<int, int> inPCell, uint32_t uuidCompany) {
        auto p = gLayerOwnership->get({inPCell.first, inPCell.second});
        p.push_front(uuidCompany); //AAAAAAAAAAAAAAAAAAAAAA
        gLayerOwnership->set(inPCell, p);
        //TODO no m'agrada que aixo vulgi dir que estem creant una copia.
    }

    void removeRefPosOwnCompany(std::pair<int,int> inPCell, uint32_t uuidCompany){
        auto p = gLayerOwnership->get({inPCell.first, inPCell.second});
        p.remove(uuidCompany); //AAAAAAAAAAAAAAAAAAAAAA
        gLayerOwnership->set(inPCell, p);
        //TODO no m'agrada que aixo vulgi dir que estem creant una copia.
    }


    std::shared_ptr<objCompany> getCompanyByUUID(uint32_t inIndex) {
        return sCT_vTotalComp.getDestByComp(inIndex);
    }

    std::vector<objCompany> getVecCompByUUID(const std::list<uint32_t> &inLUuidComp) {
        std::vector<objCompany> r;
        for (const uint32_t l: inLUuidComp) {
            r.push_back(*getCompanyByUUID(l));
        }
        return r;
    }

    std::vector<std::shared_ptr<objCompany>> getVecCompBankruptcy(const uint32_t inTDate) {
        return sCT_vTotalComp.getTotalBankruptcy(inTDate);
    }

    void applyEcoWeek() {
        sCT_vTotalComp.applyEcoWeek();
    }

    void applyEcoMonth() {
        sCT_vTotalComp.applyEcoMonth();
    }

    void applyEcoYear() {
        sCT_vTotalComp.applyEcoYear();
    }

    std::vector<sCompanyCompiler::sCCIntentions>
    getTotalIntentions(uint32_t gTimer) { return sCT_vTotalComp.getTotalIntentions(sCT_sCodeS, gTimer); }

    std::vector<std::pair<uint32_t, int>>
    getDiffEmployeesByLocation(uint32_t inRTimer) { return sCT_vTotalComp.getDiffEmployeesByLocation(inRTimer); }


private:
    rVectorCompanies sCT_vTotalComp;
    std::shared_ptr<sCodeStorage> sCT_sCodeS;
    std::mt19937 sCT_genRand;

    std::shared_ptr<gIGrid<std::list<uint32_t>>> gLayerOwnership;

    std::vector<std::vector<bool>> sCT_vActiveDaysValid = {{true,  true,  true,  true,  true, false, false},
                                                           {true,  true,  true,  true,  true, true,  false},
                                                           {false, false, false, false, true, true,  true}};
    std::vector<std::pair<uint32_t, uint32_t>> sCT_vActiveHoursValid = {{108, 204},
                                                                        {96,  192},
                                                                        {120, 216},
                                                                        {216, 276}};
};

#endif //CITYOFWEIRDFISHES_SCOMPANYSTORAGE_H

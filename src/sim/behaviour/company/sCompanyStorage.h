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
#include "../iVectorStorage.h"

class rVectorCompanies : public iVectorStorage<objCompany> {
public:

    explicit rVectorCompanies(uint32_t nMaxCompanies) : iVectorStorage(nMaxCompanies) {}

    std::vector<sCompanyCompiler::sCCIntentions>
    getTotalIntentions(const std::shared_ptr<sCodeStorage> &sSCode, uint32_t tTimer) {
        std::vector<sCompanyCompiler::sCCIntentions> ret;
        for (auto &i: iVS_tVecStorage) {
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
        for (auto &i: iVS_tVecStorage)
            if (i.second != nullptr && i.second->c_cActiveFunds < 0 &&
                (i.second->c_objYear + i.second->c_objMonth * 12 + i.second->c_objWeek * 36) < 0)
                ret.push_back(i.second);

        return ret;
    }

    void applyEcoWeek() {
        for (auto &i: iVS_tVecStorage)
            if (i.second != nullptr)
                i.second->c_cActiveFunds += i.second->c_objWeek;
    }

    void applyEcoMonth() {
        for (auto &i: iVS_tVecStorage)
            if (i.second != nullptr)
                i.second->c_cActiveFunds += i.second->c_objMonth;
    }

    void applyEcoYear() {
        for (auto &i: iVS_tVecStorage)
            if (i.second != nullptr)
                i.second->c_cActiveFunds += i.second->c_objYear;
    }

    std::vector<std::pair<uint32_t, int>> getDiffEmployeesByLocation(uint32_t inRTimer) {
        std::vector<std::pair<uint32_t, int>> ret;
        for (auto &i: iVS_tVecStorage) {
            if (i.second != nullptr) {
                std::pair<uint32_t, uint32_t> pairTimeActive = i.second->c_activeDates.c_StrEndTime;
                if (i.second->c_attrCanEmployee && inRTimer >= pairTimeActive.first &&
                    inRTimer <= pairTimeActive.second &&
                    i.second->c_nEmployee < i.second->getNumberActiveCells() * 2)
                    ret.emplace_back(i.second->c_uuid, i.second->getNumberActiveCells() * 2 - i.second->c_nEmployee);
            }
        }
        return ret;
    }

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

    uint32_t createCompany(const std::list<std::pair<int, int>> &vecNPos, uint8_t typeSoilCompany) {
        std::uniform_int_distribution<> disDay(0, (int) sCT_vActiveDaysValid.size() - 1);
        std::discrete_distribution<> disHour(sCT_vActiveHoursWeight.begin(), sCT_vActiveHoursWeight.end());
        int randomIndexDay = disDay(sCT_genRand);
        int randomIndexHour = disHour(sCT_genRand);

        std::shared_ptr newObjCompany = std::make_shared<objCompany>(objCompany(vecNPos, typeSoilCompany,
                                                                                {sCT_vActiveDaysValid[randomIndexDay],
                                                                                 sCT_vActiveHoursValid[randomIndexHour]}));
        uint32_t _idNewComp = sCT_vTotalComp.storeElement(newObjCompany);
        newObjCompany->c_uuid = _idNewComp;
        newObjCompany->c_cActiveFunds.set(5000);

        sCT_sCodeS->initNewCode(_idNewComp, typeSoilCompany);
        getCompanyByUUID(_idNewComp)->c_cCode = sCT_sCodeS->getPointerCodeByUuid(_idNewComp);


        for (const auto &nPos: vecNPos)
            addRefPosOwnCompany(nPos, _idNewComp);

        return _idNewComp;
    }

    void removeCompany(const std::shared_ptr<objCompany>& inObjCompany){
        sCT_vTotalComp.removeElement(inObjCompany->c_uuid);
    }

    bool isCompanyInPosition(const std::pair<int, int> &inPCell) { return !gLayerOwnership->get(inPCell).empty(); }

    void addRefPosOwnCompany(std::pair<int, int> inPCell, uint32_t uuidCompany) {
        auto p = gLayerOwnership->get({inPCell.first, inPCell.second});
        p.push_front(uuidCompany); //AAAAAAAAAAAAAAAAAAAAAA
        gLayerOwnership->set(inPCell, p);
        //TODO no m'agrada que aixo vulgi dir que estem creant una copia.
    }

    void removeRefPosOwnCompany(std::pair<int, int> inPCell, uint32_t uuidCompany) {
        auto p = gLayerOwnership->get({inPCell.first, inPCell.second});
        p.remove(uuidCompany); //AAAAAAAAAAAAAAAAAAAAAA
        gLayerOwnership->set(inPCell, p);
        //TODO no m'agrada que aixo vulgi dir que estem creant una copia.
    }


    std::shared_ptr<objCompany> getCompanyByUUID(uint32_t inIndex) {
        return sCT_vTotalComp.getElementByUuid(inIndex);
    }

    std::shared_ptr<objCompany> getCompanyByPosition(std::pair<int, int> inPCell) {
        return getCompanyByUUID(gLayerOwnership->get(inPCell).front());
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


    std::shared_ptr<gIGrid<std::list<uint32_t>>> gLayerOwnership;
private:
    rVectorCompanies sCT_vTotalComp;
    std::shared_ptr<sCodeStorage> sCT_sCodeS;
    std::mt19937 sCT_genRand;

    std::vector<std::vector<bool>> sCT_vActiveDaysValid = {{true,  true,  true,  true,  true, false, false},
                                                           {true,  true,  true,  true,  true, true,  false},
                                                           {false, false, false, false, true, true,  true}};
    std::vector<std::pair<uint32_t, uint32_t>> sCT_vActiveHoursValid = {{108, 204},
                                                                        {96,  192},
                                                                        {120, 216},
                                                                        {216, 276}};
    std::vector<double> sCT_vActiveHoursWeight = {0.45, 0.45, 0.1};
};

#endif //CITYOFWEIRDFISHES_SCOMPANYSTORAGE_H

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

    explicit rVectorCompanies(uint32_t inNMaxCompanies) : iVectorStorage(inNMaxCompanies) {}

    std::vector<sCompanyCompiler::sCCIntentions>
    getTotalIntentions(const std::shared_ptr<sCodeStorage> &inSCode, uint32_t inRTimer) {
        std::vector<sCompanyCompiler::sCCIntentions> ret;
        for (auto &i: iVS_tVecStorage) {
            if (i.second != nullptr) {
                std::pair<uint32_t, uint32_t> pairTimeActive = i.second->c_activeDates.c_StrEndTime;
                if (inRTimer >= pairTimeActive.first && inRTimer <= pairTimeActive.second) {
                    std::vector<sCompanyCompiler::sCCIntentions> rComp =
                            sCompanyCompiler::givenCode(inSCode->getCodeByUuid(i.second->c_uuid).sCO_Code, i.second);
                    ret.insert(ret.end(), rComp.begin(), rComp.end());
                }
            }
        }
        return ret;
    }

    std::vector<std::shared_ptr<objCompany>>
    getTotalBankruptcy(uint32_t inRTimer) {
        std::vector<std::shared_ptr<objCompany>> ret;
        for (auto &i: iVS_tVecStorage)
            if (i.second != nullptr && i.second->c_cActiveFunds < 0 &&
                (i.second->c_objYear + i.second->c_objMonth * 12 + i.second->c_objWeek * 36) < 0)
                ret.push_back(i.second);

        return ret;
    }

    void applyObligations(const objCompany::objC_TimeObligations inTypeObligation, const uint32_t &inTDate) {
        for (auto &i: iVS_tVecStorage)
            if (i.second != nullptr)
                i.second->complyTimeObligations(inTypeObligation, inTDate);
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
    explicit sCompanyStorage(uint32_t inGridSize, uint32_t inMaxComp)
            : sCT_vTotalComp(inMaxComp), sCT_sCodeS(std::make_shared<sCodeStorage>()) {

        sCT_gLayerOwnership = std::make_shared<gBasicGrid<std::list<uint32_t>>>
                (gBasicGrid<std::list<uint32_t>>(inGridSize, inGridSize, {}));

        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            sCT_genRand.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            sCT_genRand.seed(static_cast<unsigned int>(time(nullptr)));
    }

    uint32_t
    createCompany(const std::list<std::pair<int, int>> &inVecNPos, uint8_t inTypeSoilCompany, const uint32_t inRTime,
                  const uint32_t inTDate) {
        std::uniform_int_distribution<> disDay(0, (int) sCT_vActiveDaysValid.size() - 1);
        std::discrete_distribution<> disHour(sCT_vActiveHoursWeight.begin(), sCT_vActiveHoursWeight.end());
        int randomIndexDay = disDay(sCT_genRand);
        int randomIndexHour = disHour(sCT_genRand);

        std::shared_ptr newObjCompany = std::make_shared<objCompany>(objCompany(inVecNPos, inTypeSoilCompany,
                                                                                {sCT_vActiveDaysValid[randomIndexDay],
                                                                                 sCT_vActiveHoursValid[randomIndexHour]}));
        uint32_t _idNewComp = sCT_vTotalComp.storeElement(newObjCompany);
        newObjCompany->c_uuid = _idNewComp;
        newObjCompany->addPayment(5000, oPC_TypePayment::oPC_TP_INVESTMENT_START,inRTime,inTDate);
        sCT_sCodeS->initNewCode(_idNewComp, inTypeSoilCompany);
        getCompanyByUUID(_idNewComp)->c_cCode = sCT_sCodeS->getPointerCodeByUuid(_idNewComp);


        for (const auto &nPos: inVecNPos)
            addRefPosOwnCompany(nPos, _idNewComp);

        return _idNewComp;
    }

    void removeCompany(const std::shared_ptr<objCompany> &inObjCompany) {
        sCT_vTotalComp.removeElement(inObjCompany->c_uuid);
    }

    bool isCompanyInPosition(const std::pair<int, int> &inPCell) { return !sCT_gLayerOwnership->get(inPCell).empty(); }

    void addRefPosOwnCompany(std::pair<int, int> inPCell, uint32_t inUuidCompany) {
        std::list<uint32_t> p = sCT_gLayerOwnership->get({inPCell.first, inPCell.second});
        p.push_front(inUuidCompany);
        sCT_gLayerOwnership->set(inPCell, p);
    }

    void removeRefPosOwnCompany(std::pair<int, int> inPCell, uint32_t inUuidCompany) {
        auto p = sCT_gLayerOwnership->get({inPCell.first, inPCell.second});
        p.remove(inUuidCompany);
        sCT_gLayerOwnership->set(inPCell, p);
    }

    void updateScoreCode(const uint32_t  inUuidCompany,const int inVChange){
        sCT_sCodeS->updateScoreCode(inUuidCompany, inVChange);
    }

    std::shared_ptr<objCompany> getCompanyByUUID(uint32_t inIndex) {
        return sCT_vTotalComp.getElementByUuid(inIndex);
    }

    std::shared_ptr<objCompany> getCompanyByPosition(std::pair<int, int> inPCell) {
        return getCompanyByUUID(sCT_gLayerOwnership->get(inPCell).front());
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

    void applyObligations(const objCompany::objC_TimeObligations inTypeObligation, const uint32_t &inTDate) {
        sCT_vTotalComp.applyObligations(inTypeObligation, inTDate);
    }

    std::vector<sCompanyCompiler::sCCIntentions>
    getTotalIntentions(uint32_t inRTimer) { return sCT_vTotalComp.getTotalIntentions(sCT_sCodeS, inRTimer); }

    std::vector<std::pair<uint32_t, int>>
    getDiffEmployeesByLocation(uint32_t inRTimer) { return sCT_vTotalComp.getDiffEmployeesByLocation(inRTimer); }


    std::shared_ptr<gIGrid<std::list<uint32_t>>> sCT_gLayerOwnership;
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

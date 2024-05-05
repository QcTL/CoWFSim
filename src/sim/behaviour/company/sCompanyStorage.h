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

/**
 * @class rVectorCompanies
 * @brief This class implements the vectorStorage for the object "objCompany"
 */
class rVectorCompanies : public iVectorStorage<objCompany> {
public:

    explicit rVectorCompanies(uint32_t inNMaxCompanies) : iVectorStorage(inNMaxCompanies) {}

    /**
     * @fn std::vector<sCompanyCompiler::sCCIntentions> getTotalIntentions
     * @brief returns all the sCCIntentions of all the companies stored
     * @param inSCode The class that stores the code for each company
     * @param inRTimer the current reduced time in a simulation < 288
     * @return
     */
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

    /**
     * @fn std::vector<std::shared_ptr<objCompany>> getTotalBankruptcy
     * @return Returns a list of all the companies that are currently in bankruptcy, so that they have negative liquid money
     * and a negative cash flow for the next year
     */
    std::vector<std::shared_ptr<objCompany>> getTotalBankruptcy() {
        std::vector<std::shared_ptr<objCompany>> ret;
        for (auto &i: iVS_tVecStorage)
            if (i.second != nullptr && i.second->c_cActiveFunds < 0 &&
                (i.second->c_objYear + i.second->c_objMonth * 12 + i.second->c_objWeek * 36) < 0)
                ret.push_back(i.second);

        return ret;
    }

    /**
     * @fn void applyObligations
     * @brief Reduces the current liquid money of the company to pay the obligations in the different periods
     * @param inTypeObligation An enumerator detailing the type of time obligation it has to comply
     * @param inTDate The current reduced date from the simulation, it has to be valid
     */
    void applyObligations(const objCompany::objC_TimeObligations inTypeObligation, const uint32_t &inTDate) {
        for (auto &i: iVS_tVecStorage)
            if (i.second != nullptr)
                i.second->complyTimeObligations(inTypeObligation, inTDate);
    }

    /**
     * @fn std::vector<std::pair<uint32_t, int>> getDiffEmployeesByLocation
     * @brief get list detailing for each company how many employees it has
     * @param inRTimer The reduced time of the simulation < 288
     * @return A vector with the uuid of a company and how many employees it has
     */
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

/**
 * @class sCompanyStorage
 * @brief This class is the one responsible for administrating the rVectorCompanies and the stored companies
 */
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

    /**
     * @fn uint32_t createCompany
     * @param inVecNPos The list of positions this company has, it cannot be an occupation one
     * @param inTypeSoilCompany The type of company that is going to be created
     * @param inRTime The simulated Reduced time where the event was fired, < 288
     * @param inTDate The simulated Reduced date where the event was fired, it has to be a valid reduced date.
     * @return
     */
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
        newObjCompany->addPayment(5000, oPC_TypePayment::oPC_TP_INVESTMENT_START, inRTime, inTDate);
        sCT_sCodeS->initNewCode(_idNewComp, inTypeSoilCompany);
        getCompanyByUUID(_idNewComp)->c_cCode = sCT_sCodeS->getPointerCodeByUuid(_idNewComp);


        for (const auto &nPos: inVecNPos)
            addRefPosOwnCompany(nPos, _idNewComp);

        return _idNewComp;
    }

    /**
     * @fn void removeCompany
     * @param inObjCompany A valid company that hasn't been removed, that you want to have removed
     */
    void removeCompany(const std::shared_ptr<objCompany> &inObjCompany) {
        sCT_vTotalComp.removeElement(inObjCompany->c_uuid);
    }

    /**
     * @fn bool isCompanyInPosition
     * @brief Get if in that position, a company is present
     * @param inPCell A Positive pair of coordinates inside the simulation board
     * @return Returns true if there is a company inside the position, false otherwise
     */
    bool isCompanyInPosition(const std::pair<int, int> &inPCell) { return !sCT_gLayerOwnership->get(inPCell).empty(); }

    /**
     * @fn void addRefPosOwnCompany
     * @brief Adds in the object of the company, a new cell in its list of ownership
     * @param inPCell A Positive pair of coordinates inside the simulation board
     * @param inUuidCompany A valid uuid of a created company not yet removed
     */
    void addRefPosOwnCompany(std::pair<int, int> inPCell, uint32_t inUuidCompany) {
        std::list<uint32_t> p = sCT_gLayerOwnership->get({inPCell.first, inPCell.second});
        p.push_front(inUuidCompany);
        sCT_gLayerOwnership->set(inPCell, p);
    }

    /**
     * @fn void removeRefPosOwnCompany
     * @brief Remove from the object of the company, the cell of its its of ownership
     * @param inPCell A Positive pair of coordinates inside the simulation board
     * @param inUuidCompany A valid uuid of a created company not yet removed
     */
    void removeRefPosOwnCompany(std::pair<int, int> inPCell, uint32_t inUuidCompany) {
        auto p = sCT_gLayerOwnership->get({inPCell.first, inPCell.second});
        p.remove(inUuidCompany);
        sCT_gLayerOwnership->set(inPCell, p);
    }

    /**
     * @fn void updateScoreCode
     * @brief change the  value of the score that the company by a given amount, specified in the parameters
     * @param inUuidCompany  A valid uuid of a created company not yet removed
     * @param inVChange The value that you want the score of the company to change
     */
    void updateScoreCode(const uint32_t inUuidCompany, const int inVChange) {
        sCT_sCodeS->updateScoreCode(inUuidCompany, inVChange);
    }

    /**
     * @fn   std::shared_ptr<objCompany> getCompanyByUUID
     * @param inUuidCompany A valid uuid of a created company not yet removed
     * @return A shared pointer of the company with that uuid given as a parameter
     */
    std::shared_ptr<objCompany> getCompanyByUUID(uint32_t inUuidCompany) {
        return sCT_vTotalComp.getElementByUuid(inUuidCompany);
    }

    /**
     * @fn  std::shared_ptr<objCompany> getCompanyByPosition
     * @param inPCell A Positive pair of coordinates inside the simulation board, that contains a company
     * @return  A shared pointer of the company residing in the position that the parameter refers
     */
    std::shared_ptr<objCompany> getCompanyByPosition(std::pair<int, int> inPCell) {
        return getCompanyByUUID(sCT_gLayerOwnership->get(inPCell).front());
    }


    /**
     * @fn std::vector<std::shared_ptr<objCompany>> getTotalBankruptcy
     * @return Returns a list of all the companies that are currently in bankruptcy, so that they have negative liquid money
     * and a negative cash flow for the next year
     */
    std::vector<std::shared_ptr<objCompany>> getVecCompBankruptcy() {
        return sCT_vTotalComp.getTotalBankruptcy();
    }

    /**
 * @fn void applyObligations
 * @brief Reduces the current liquid money of the company to pay the obligations in the different periods
 * @param inTypeObligation An enumerator detailing the type of time obligation it has to comply
 * @param inTDate The current reduced date from the simulation, it has to be valid
 */
    void applyObligations(const objCompany::objC_TimeObligations inTypeObligation, const uint32_t &inTDate) {
        sCT_vTotalComp.applyObligations(inTypeObligation, inTDate);
    }

    /**
     * @fn std::vector<sCompanyCompiler::sCCIntentions> getTotalIntentions
     * @brief returns all the sCCIntentions of all the companies stored
     * @param inRTimer the current reduced time in a simulation < 288
     * @return
     */
    std::vector<sCompanyCompiler::sCCIntentions>
    getTotalIntentions(uint32_t inRTimer) { return sCT_vTotalComp.getTotalIntentions(sCT_sCodeS, inRTimer); }

    /**
     * @fn std::vector<std::pair<uint32_t, int>> getDiffEmployeesByLocation
     * @brief get list detailing for each company how many employees it has
     * @param inRTimer The reduced time of the simulation < 288
     * @return A vector with the uuid of a company and how many employees it has
     */
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

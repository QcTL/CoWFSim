//
// Created by ganymede on 4/1/24.
//

#ifndef CITYOFWEIRDFISHES_GROUPECONOMY_H
#define CITYOFWEIRDFISHES_GROUPECONOMY_H

#include <memory>
#include "../../behaviour/market/sMarketBazaar.h"
#include "../../behaviour/market/sEvaluatorMain.h"
#include "../../behaviour/market/listings/sRollingListsRent.h"

class groupEconomy {
public:
    groupEconomy() = default;

    void setEmptySlots(const std::list<sgTerrain::sgT_CellSlot> &inLEmpty) {
        for (const sgTerrain::sgT_CellSlot &sCellSlot: inLEmpty) {
            for (const auto &pElem: sCellSlot.sgT_gPos) {
                std::shared_ptr<sLBuyCell::sMOffering> sMO =
                        std::make_shared<sLBuyCell::sMOffering>(nullptr, pElem,
                                                                sCellSlot.sgT_soilType,
                                                                0, 0); //TODO PRICE AND QUALITY
                addListing<sLBuyCell>(sMO);
            }
        }
    }

    /**
     * @fn void tickReduced
     * @brief Updates the elements in the class that need changing every 5 tick of the simulation
     * @param inRTime uint representing the reduced time of the day
     * @param inTDate uint representing the reduced date of the simulation
     */
    void tickReduced(const uint32_t inRTime, const uint32_t inTDate) {
        gE_sEvaluator->tickReduced(inRTime, inTDate);
    }

    // FUNCTIONS BAZAAR;
    template<typename T>
    std::shared_ptr<typename T::sMOffering> getListOfOffering(typename T::sMFilter &sMFilter) {
        return gE_sMarketBazaar->getListOfOffering(sMFilter);
    }

    template<typename T>
    void removeCompleteProcess(std::shared_ptr<typename T::sMOffering> &inSMOffer) {
        gE_sMarketBazaar->removeCompleteProcess(inSMOffer);
    }

    template<typename T>
    void addListing(std::shared_ptr<typename T::sMOffering> &inSMOffer) {
        gE_sMarketBazaar->addListing(inSMOffer);
    }

    // FUNCTIONS EVALUATOR
    void computeCreatedElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany) {
        gE_sEvaluator->computeCreatedElement(inUuidElement, inObjCompany);
    }

    void computeBoughtElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                              uint32_t inCDate) {
        gE_sEvaluator->computeBoughtElement(inUuidElement, inObjCompany, inRTime, inCDate);
    }

    void
    computeBoughtElementCiv(uint64_t inUuidElement, uint32_t inQuantityElement, uint32_t inRTime, uint32_t inCDate) {
        gE_sEvaluator->computeBoughtElementCiv(inUuidElement, inQuantityElement, inRTime, inCDate);
    }

    void
    computeSellInmElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                          uint32_t inCDate) {
        gE_sEvaluator->computeSellInmElement(inUuidElement, inObjCompany, inRTime, inCDate);
    }

    bool doesObjectExists(const uint32_t uuidItem) {
        return gE_sEvaluator->doesObjectExists(uuidItem);
    }


    [[nodiscard]] std::vector<std::pair<std::string, uint64_t>> getVecAllItemsPrices() const{
        return gE_sEvaluator->getVecAllItemsPrices();
    }

    void changeStateEyeProduct(const uint32_t inUuidProduct){
        gE_sEvaluator->changeStateEye(inUuidProduct);
    }

    bool getStateEyeProduct(const uint32_t inUuidProduct){
        return gE_sEvaluator->getStateEye(inUuidProduct);
    }

    sTotalElements::sME_Element getById(uint64_t inUuidElement) { return gE_sEvaluator->getById(inUuidElement); }

    std::string getNameById(uint64_t inUuidElement){return gE_sEvaluator->getStringByObjId(inUuidElement);}

    //Parameters
    std::shared_ptr<sRollingListsRent> gE_sRLR = std::make_shared<sRollingListsRent>(10);
    std::shared_ptr<sEvaluatorMain> gE_sEvaluator = std::make_shared<sEvaluatorMain>();
    std::shared_ptr<sMarketBazaar> gE_sMarketBazaar = std::make_shared<sMarketBazaar>();
};

#endif //CITYOFWEIRDFISHES_GROUPECONOMY_H

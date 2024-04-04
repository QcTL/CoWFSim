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
            std::shared_ptr<sLBuyCell::sMOffering> sMO =
                    std::make_shared<sLBuyCell::sMOffering>(nullptr, sCellSlot.sgT_gPos,
                                                            sCellSlot.sgT_gType,
                                                            0, 0); //TODO PRICE AND QUALITY
            addListing<sLBuyCell>(sMO);
        }
    }

    void tickReduced(const uint32_t inRTime, const uint32_t inTDate) {
        gE_sEvaluator->tickReduced(inRTime, inTDate);
    }

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

    void computeCreatedElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany) {
        gE_sEvaluator->computeCreatedElement(inUuidElement, inObjCompany);
    }

    void computeBoughtElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany) {
        gE_sEvaluator->computeCreatedElement(inUuidElement, inObjCompany);
    }

    void computeSellInmElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany) {
        gE_sEvaluator->computeSellInmElement(inUuidElement, inObjCompany);
    }

    sTotalElements::sME_Element getById(uint64_t inUuidElement) { return gE_sEvaluator->getById(inUuidElement); }

    std::shared_ptr<sRollingListsRent> gE_sRLR = std::make_shared<sRollingListsRent>(10);
    std::shared_ptr<sEvaluatorMain> gE_sEvaluator = std::make_shared<sEvaluatorMain>();
    std::shared_ptr<sMarketBazaar> gE_sMarketBazaar = std::make_shared<sMarketBazaar>();
};

#endif //CITYOFWEIRDFISHES_GROUPECONOMY_H

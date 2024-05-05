//
// Created by ganymede on 4/26/24.
//

#ifndef CITYOFWEIRDFISHES_SOBJPRODUCT_H
#define CITYOFWEIRDFISHES_SOBJPRODUCT_H

#include <cstdint>
#include <utility>
#include "../../../eyeCatcher/eyeValue.h"
#include "../../../structure/obj/elements/objCompany.h"
#include "sRollingListsEvaluator.h"
#include "sTotalElements.h"
#include "../../../events/sEventManager.h"

/**
 * @class sObjProduct
 * @brief This class represents a product that can be created by some company
 */
class sObjProduct {
public:
    sObjProduct(const std::string &inNameProduct, const uint32_t startBasePrice, const uint32_t startBuyDesire,
                sTotalElements::sME_Element inElem)
            : sP_desirability(startBuyDesire), sP_basePrice(startBasePrice), sP_pastPrice(startBasePrice),
              sP_fluctuationPrice("Prod_" + inNameProduct + "-mPrice", startBasePrice),
              sP_refElem(std::move(inElem)) {
        sP_lastNTransactions = std::make_shared<sRollingListsEvaluator>(5);
    }

    /**
     * @fn uint32_t getPriceActual
     * @brief Using supply and demand return what should be the price of that product
     * @return The price of the product this clas represent in this instance
     */
    uint32_t getPriceActual() {
        uint32_t _objPrice = sP_refElem.getPrice(sP_lastNTransactions->getDesirability(), sP_basePrice);

        sP_fluctuationPrice.set(_objPrice);

        if (sP_companyHasItem.empty())
            return (uint32_t) (1.1 * _objPrice);
        return std::max((uint32_t) 1, _objPrice);
    }

    /**
     * @fn void computeCreatedElementCOMPANY
     * @brief This function should be called when this product is created by a company
     * @param inObjCompany The pointer company that created the item, has to be a valid company
     */
    void computeCreatedElementCOMPANY(const std::shared_ptr<objCompany> &inObjCompany) {
        sP_lastNTransactions->addLastCreate();

        diffElementCompany(1, inObjCompany);
        sP_companyHasItem.push_back(inObjCompany);
    }

    /**
     * @fn void computeBoughtElementCOMPANY
     * @brief This function should be called everytime that product is purchased by a company
     * @param inObjCompany The company that purchased the product
     * @param inRTime The reduced time of the actual simulation < 288
     * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
     */
    void computeBoughtElementCOMPANY(const std::shared_ptr<objCompany> &inObjCompany,
                                     const uint32_t inRTime, const uint32_t inCDate) {
        uint32_t _pItem = getPriceActual();
        sP_lastNTransactions->addLastBought();

        if (!sP_companyHasItem.empty())
            _takeAndPayObjectLocalComp(inRTime, inCDate);
        else
            sEventManager::getInstance()->callEventCompanyBoughtItemImport(inRTime, inCDate, inObjCompany->c_uuid,
                                                                           sP_refElem.sMEE_uuid, _pItem);

        diffElementCompany(1, inObjCompany);
        inObjCompany->addPayment((double) _pItem * -1, oPC_TypePayment::oPC_TP_SOLD, inRTime, inCDate);
    }

    /**
     * @fn void computeBoughtElementCIVIL
     * @brief This function should be called everytime that product is purchased by a civilian
     * @param inRTime The reduced time of the actual simulation < 288
     * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
     */
    void computeBoughtElementCIVIL(const uint32_t inRTime, const uint32_t inCDate) {
        sP_lastNTransactions->addLastBought();

        if (!sP_companyHasItem.empty())
            _takeAndPayObjectLocalComp(inRTime, inCDate);
    }

    /**
     * @fn uint32_t getNProductsDesirability
     * @brief Get the product desirability given the quantity of active population on the simulation
     * @param inNPopulation The quantity of people in the simulation in that moment
     * @return A number representing the desirability of that item, 0 is none.
     */
    [[nodiscard]] uint32_t getNProductsDesirability(const uint32_t inNPopulation) const {
        //f(x)=((3)/(1+ℯ^(-((x)/(5)))))-0.5
        return (uint32_t) (((double) inNPopulation / sP_desirability) *
                           std::max(1.0, (3.0 / (1.0 + std::exp(-getDifferencePrice() / 5.0))) - 0.5));
    }

    void computeSellInm(const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime, uint32_t inCDate) {
        uint32_t _pItem = getPriceActual();

        diffElementCompany(-1, inObjCompany);
        inObjCompany->addPayment((double) _pItem, oPC_TypePayment::oPC_TP_SOLD, inRTime, inCDate);
        sEventManager::getInstance()->callEventCompanySoldItemExport(inRTime, inCDate, inObjCompany->c_uuid,
                                                                     sP_refElem.sMEE_uuid, _pItem);
    }

    void takeAndPayObjectLocalComp(const uint32_t inRTime, const uint32_t inCDate) {
        std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();

        uint32_t _pItem = getPriceActual();
        diffElementCompany(-1, sP_companyHasItem.front());
        sP_companyHasItem.front()->addPayment((double) _pItem, oPC_TypePayment::oPC_TP_SOLD, inRTime,
                                              inCDate);
        sEM->callEventCompanySoldItemLocal(inRTime, inCDate, sP_companyHasItem.front()->c_uuid,
                                           sP_refElem.sMEE_uuid, _pItem);

        sP_companyHasItem.pop_front();
    }

    bool getStateEye() { return sP_fluctuationPrice.isObserved(); }

    void changeStateEye() {
        sP_fluctuationPrice.isObserved()
        ? sP_fluctuationPrice.removeObserver() : sP_fluctuationPrice.setObserver(eyeCatcherActive::getInstance());
    }

    void newValueBase() {
        sP_basePrice = sP_refElem.getPrice(sP_lastNTransactions->dropLastWindow(), sP_basePrice);
    }

    void newValuePast() {
        sP_pastPrice = sP_basePrice;
    }

private:
    uint32_t sP_basePrice;
    eyeValue<uint32_t> sP_fluctuationPrice;
    uint32_t sP_pastPrice;
    uint32_t sP_desirability;
    std::list<std::shared_ptr<objCompany>> sP_companyHasItem;
    std::shared_ptr<sRollingListsEvaluator> sP_lastNTransactions;
    sTotalElements::sME_Element sP_refElem;

    void diffElementCompany(int inNElements, const std::shared_ptr<objCompany> &inPObjCompany) const {
        if (inPObjCompany != nullptr) {
            if (!inPObjCompany->c_pOwn.count(sP_refElem.sMEE_uuid))
                inPObjCompany->c_pOwn[sP_refElem.sMEE_uuid] = inNElements;
            else
                inPObjCompany->c_pOwn[sP_refElem.sMEE_uuid] += inNElements;
        }
    }

    void _takeAndPayObjectLocalComp(const uint32_t inRTime, const uint32_t inCDate) {
        std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();

        const uint32_t _pItem = getPriceActual();
        diffElementCompany(-1, sP_companyHasItem.front());
        sP_companyHasItem.front()->addPayment((double) _pItem, oPC_TypePayment::oPC_TP_SOLD, inRTime, inCDate);
        sEM->callEventCompanySoldItemLocal(inRTime, inCDate, sP_companyHasItem.front()->c_uuid,
                                           sP_refElem.sMEE_uuid, _pItem);
        sP_companyHasItem.pop_front();
    }

    [[nodiscard]] int getDifferencePrice() const {
        return (int) sP_pastPrice - (int) sP_basePrice;
    }

};

#endif //CITYOFWEIRDFISHES_SOBJPRODUCT_H

//
// Created by ganymede on 4/26/24.
//

#ifndef CITYOFWEIRDFISHES_SOBJPRODUCT_H
#define CITYOFWEIRDFISHES_SOBJPRODUCT_H

#include <cstdint>
#include "../../../eyeCatcher/eyeValue.h"
#include "../../../structure/obj/elements/objCompany.h"
#include "sRollingListsEvaluator.h"
#include "sTotalElements.h"
#include "../../../events/sEventManager.h"

class sObjProduct {
public:
    sObjProduct(const std::string &inNameProduct, const uint32_t startBasePrice, const uint32_t startBuyDesire,
                const sTotalElements::sME_Element& inElem)
            : sP_desirability(startBuyDesire), sP_basePrice(startBasePrice), sP_pastPrice(startBasePrice),
              sP_fluctuationPrice("Prod_" + inNameProduct + "-mPrice", startBasePrice),
              sP_refElem(inElem) {
        sP_lastNTransactions = std::make_shared<sRollingListsEvaluator>(5);
    }

    uint32_t getPriceActual() {
        uint32_t _objPrice = sP_refElem.getPrice(sP_lastNTransactions->getDesirability(), sP_basePrice);

        sP_fluctuationPrice.set(_objPrice);

        if (sP_companyHasItem.empty())
            return (uint32_t) (1.1 * _objPrice);
        return std::max((uint32_t) 1, _objPrice);
    }

    void computeCreatedElementCOMPANY(const std::shared_ptr<objCompany> &inObjCompany) {
        sP_lastNTransactions->addLastCreate();

        diffElementCompany(1, inObjCompany);
        sP_companyHasItem.push_back(inObjCompany);
    }

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

    void computeBoughtElementCIVIL(const uint32_t inRTime, const uint32_t inCDate) {
        sP_lastNTransactions->addLastBought();

        if (!sP_companyHasItem.empty())
            _takeAndPayObjectLocalComp(inRTime, inCDate);
    }


    [[nodiscard]] uint32_t getNProductsDesirability(const uint32_t inNPopulation) const {
        //f(x)=((3)/(1+ℯ^(-((x)/(5)))))-0.5
        return (uint32_t) (((double) inNPopulation / sP_desirability) *
                           std::max(1.0, (3.0 / (1.0 + std::exp(-getDifferencePrice() / 5.0))) - 0.5));
    }

    void
    computeSellInm(const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                   uint32_t inCDate) {
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

    void diffElementCompany(int inNElements, const std::shared_ptr<objCompany> &inPObjCompany) {
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

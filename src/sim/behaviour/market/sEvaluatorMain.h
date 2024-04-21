//
// Created by ganymede on 3/21/24.
//

#ifndef CITYOFWEIRDFISHES_SEVALUATORMAIN_H
#define CITYOFWEIRDFISHES_SEVALUATORMAIN_H

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "../../../common/RelPath.h"
#include "items/sRollingListsEvaluator.h"
#include "items/sTotalElements.h"
#include "../globalAttr/stGlobalTrackerAttr.h"

class sEvaluatorMain {
public:
    sEvaluatorMain() {
        sEM_totalElements = std::make_shared<sTotalElements>("FObjProduced.csv");
        sEM_gBasicPrices = std::vector<uint32_t>(sEM_totalElements->nElements(), 100);
        sEM_vLastTransactions = std::vector<sRollingListsEvaluator>(sEM_totalElements->nElements(),
                                                                    sRollingListsEvaluator(5));
        sEM_companyHasItem = std::vector<std::list<std::shared_ptr<objCompany>>>(sEM_totalElements->nElements(),
                                                                                 std::list<std::shared_ptr<objCompany>>());

        for (int i = 0; i < sEM_totalElements->nElements(); i++) {
            sEM_eyeLastCheckedValue.push_back(eyeValue<uint32_t>("Prod_" + getStringByObjId(i) + "-mPrice",
                                                            sEM_gBasicPrices[i]));
        }
    };

    std::vector<std::pair<std::string, uint64_t>> getVecAllItemsPrices() {
        std::vector<std::pair<std::string, uint64_t>> _retVec;
        for (const uint64_t uuidObj: sEM_totalElements->extractUuids())
            _retVec.emplace_back(getStringByObjId(uuidObj), getPriceItemActual(uuidObj));

        return _retVec;
    }

    [[nodiscard]] std::string getStringByObjId(const uint32_t inIdProduct) const {
        return sEM_totalElements->getNameById(inIdProduct);
    }

    void tickReduced(uint32_t inRTick, uint32_t inTDate) {
        if (inRTick == 0)
            updateStaticElements();
    }

    void computeCreatedElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany) {
        sEM_gAvailableItems[inUuidElement] += 1;
        sEM_vLastTransactions[inUuidElement].addLastCreate();

        diffElementCompany(inUuidElement, 1, inObjCompany);
        sEM_companyHasItem[inUuidElement].push_back(inObjCompany);
    }

    void
    computeBoughtElementCiv(uint64_t inUuidElement, uint32_t inQuantityElement, uint32_t inRTime, uint32_t inCDate) {
        for (int i = 0; i < inQuantityElement; i++) {
            sEM_vLastTransactions[inUuidElement].addLastBought();
            if (sEM_gAvailableItems.find(inUuidElement) != sEM_gAvailableItems.end() &&
                sEM_gAvailableItems[inUuidElement] > 0) {
                _takeAndPayObjectLocalComp(inUuidElement, inRTime, inCDate);

                sEM_companyHasItem[inUuidElement].pop_front();
            }
        }
    }

    void computeBoughtElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                              uint32_t inCDate) {
        uint32_t _pItem = getPriceItemActual(inUuidElement);
        sEM_vLastTransactions[inUuidElement].addLastBought();
        std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();

        if (sEM_gAvailableItems.find(inUuidElement) != sEM_gAvailableItems.end() &&
            sEM_gAvailableItems[inUuidElement] > 0) {
            _takeAndPayObjectLocalComp(inUuidElement, inRTime, inCDate);
            sEM_companyHasItem[inUuidElement].pop_front();
        } else {
            sEM->callEventCompanyBoughtItemImport(inRTime, inCDate, inObjCompany->c_uuid, inUuidElement, _pItem);
            std::cout << _pItem << " : Importat" << std::endl;
        }
        diffElementCompany(inUuidElement, 1, inObjCompany);
        inObjCompany->addPayment((double)_pItem*-1,oPC_TypePayment::oPC_TP_SOLD, inRTime, inCDate);
    }

    void _takeAndPayObjectLocalComp(uint64_t inUuidElement, uint32_t inRTime, uint32_t inCDate) {
        std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();

        uint32_t _pItem = getPriceItemActual(inUuidElement);
        diffElementCompany(inUuidElement, -1, sEM_companyHasItem[inUuidElement].front());
        sEM_companyHasItem[inUuidElement].front()->addPayment((double)_pItem,oPC_TypePayment::oPC_TP_SOLD, inRTime, inCDate);
        sEM->callEventCompanySoldItemLocal(inRTime, inCDate, sEM_companyHasItem[inUuidElement].front()->c_uuid,
                                           inUuidElement, _pItem);

        sEM_gAvailableItems[inUuidElement] -= 1;
    }

    void
    computeSellInmElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                          uint32_t inCDate) {
        uint32_t _pItem = getPriceItemActual(inUuidElement);

        diffElementCompany(inUuidElement, -1, inObjCompany);
        inObjCompany->addPayment((double)_pItem,oPC_TypePayment::oPC_TP_SOLD, inRTime, inCDate);
        sEventManager::getInstance()->callEventCompanySoldItemExport(inRTime, inCDate, inObjCompany->c_uuid,
                                                                     inUuidElement, _pItem);
    }

    sTotalElements::sME_Element getById(uint64_t inUuidElement) { return sEM_totalElements->getById(inUuidElement); }

    bool doesObjectExists(const uint32_t uuidItem) {
        return uuidItem < sEM_totalElements->nElements();
    }

    void changeStateEye(const uint32_t uuidItem) {
        if (sEM_eyeLastCheckedValue[uuidItem].isObserved())
            sEM_eyeLastCheckedValue[uuidItem].removeObserver();
        else
            sEM_eyeLastCheckedValue[uuidItem].setObserver(eyeCatcherActive::getInstance());
    }

private:

    static void
    diffElementCompany(uint64_t inUuidElement, int inNElements, const std::shared_ptr<objCompany> &inPObjCompany) {
        if (inPObjCompany != nullptr) {
            if (inPObjCompany->c_pOwn.find(inUuidElement) == inPObjCompany->c_pOwn.end())
                inPObjCompany->c_pOwn[inUuidElement] = inNElements;
            else
                inPObjCompany->c_pOwn[inUuidElement] += inNElements;
        }
    }

    uint32_t getPriceItemActual(uint64_t inUuidElement) {
        uint32_t _objPrice = sEM_totalElements->getById(inUuidElement).getPrice(
                sEM_vLastTransactions[inUuidElement].getDesirability(), sEM_gBasicPrices);
        if (sEM_eyeLastCheckedValue[inUuidElement].get() != _objPrice)
            sEM_eyeLastCheckedValue[inUuidElement].set(_objPrice);

        if (sEM_gAvailableItems[inUuidElement] <= 0)
            return (uint32_t) (1.1 * _objPrice);
        return _objPrice;
    }

    void updateStaticElements() {
        //They are update only for the information that the falled window of rolling proportions us.
        for (int i = 0; i < sEM_gBasicPrices.size(); i++)
            sEM_gBasicPrices[i] = getById(i).getPrice(sEM_vLastTransactions[i].dropLastWindow(), sEM_gBasicPrices);
        std::cout<< "strt Of the day" << std::endl;
    }

    std::shared_ptr<sTotalElements> sEM_totalElements;

    std::vector<uint32_t> sEM_gBasicPrices;
    std::vector<sRollingListsEvaluator> sEM_vLastTransactions;

    std::vector<eyeValue<uint32_t>> sEM_eyeLastCheckedValue;

    std::map<std::string, uint32_t> sEM_gItemUuidByName;
    std::map<uint64_t, uint32_t> sEM_gAvailableItems;

    std::vector<std::list<std::shared_ptr<objCompany>>> sEM_companyHasItem;

};

#endif //CITYOFWEIRDFISHES_SEVALUATORMAIN_H

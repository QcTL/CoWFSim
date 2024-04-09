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
        sEM_gBasicPrices = std::vector<uint32_t>(sEM_totalElements->nElements(), 10);
        sEM_vLastTransactions = std::vector<sRollingListsEvaluator>(sEM_totalElements->nElements(),
                                                                    sRollingListsEvaluator(5));
        sEM_companyHasItem = std::vector<std::list<std::shared_ptr<objCompany>>>(sEM_totalElements->nElements(),
                                                                                 std::list<std::shared_ptr<objCompany>>());
    };


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

    void computeBoughtElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                              uint32_t inCDate) {
        uint32_t _pItem = getPriceItemActual(inUuidElement);
        sEM_vLastTransactions[inUuidElement].addLastBought();
        std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();

        if (sEM_gAvailableItems.find(inUuidElement) != sEM_gAvailableItems.end() &&
            sEM_gAvailableItems[inUuidElement] > 0) {
            diffElementCompany(inUuidElement, -1, sEM_companyHasItem[inUuidElement].front());
            if (sEM_companyHasItem[inUuidElement].front()->c_pOwn[inUuidElement] <= 0) {
                sEM->callEventCompanySoldItemLocal(inRTime, inCDate, sEM_companyHasItem[inUuidElement].front()->c_uuid,
                                                   inUuidElement, _pItem);
                sEM_companyHasItem[inUuidElement].pop_front();
            }
            sEM_gAvailableItems[inUuidElement] -= 1;

            sEM->callEventCompanyBoughtItemLocal(inRTime, inCDate, inObjCompany->c_uuid, inUuidElement, _pItem);
        } else
            sEM->callEventCompanyBoughtItemImport(inRTime, inCDate, inObjCompany->c_uuid, inUuidElement, _pItem);

        diffElementCompany(inUuidElement, 1, inObjCompany);
        inObjCompany->c_cActiveFunds -= _pItem;
    }

    void
    computeSellInmElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                          uint32_t inCDate) {
        uint32_t _pItem = getPriceItemActual(inUuidElement);

        diffElementCompany(inUuidElement, -1, inObjCompany);
        inObjCompany->c_cActiveFunds += _pItem;
        sEventManager::getInstance()->callEventCompanySoldItemExport(inRTime, inCDate, inObjCompany->c_uuid,
                                                                     inUuidElement, _pItem);

    }

    sTotalElements::sME_Element getById(uint64_t inUuidElement) { return sEM_totalElements->getById(inUuidElement); }

    bool doesObjectExists(const uint32_t uuidItem) {
        return uuidItem < sEM_totalElements->nElements();
    }

private:

    static void
    diffElementCompany(uint64_t inUuidElement, int inNElements, const std::shared_ptr<objCompany> &inPObjCompany) {
        if (inPObjCompany->c_pOwn.find(inUuidElement) == inPObjCompany->c_pOwn.end())
            inPObjCompany->c_pOwn[inUuidElement] = inNElements;
        else
            inPObjCompany->c_pOwn[inUuidElement] += inNElements;
    }

    uint32_t getPriceItemActual(uint64_t inUuidElement) {
        if (sEM_gAvailableItems[inUuidElement] <= 0)
            return (uint32_t) (1.1 * sEM_totalElements->getById(inUuidElement).getPrice(
                    sEM_vLastTransactions[inUuidElement].getDesirability(), sEM_gBasicPrices));
        return sEM_totalElements->getById(inUuidElement).getPrice(
                sEM_vLastTransactions[inUuidElement].getDesirability(), sEM_gBasicPrices);
    }

    void updateStaticElements() {
        //They are update only for the information that the falled window of rolling proportions us.
        for (int i = 0; i < sEM_gBasicPrices.size(); i++)
            sEM_gBasicPrices[i] = uint32_t(sEM_gBasicPrices[i] * sEM_vLastTransactions[i].dropLastWindow());
    }

    std::shared_ptr<sTotalElements> sEM_totalElements;

    std::vector<uint32_t> sEM_gBasicPrices; // Updated once a day;
    std::vector<sRollingListsEvaluator> sEM_vLastTransactions;

    std::map<std::string, uint32_t> sEM_gItemUuidByName;
    std::map<uint64_t, uint32_t> sEM_gAvailableItems;

    std::vector<std::list<std::shared_ptr<objCompany>>> sEM_companyHasItem;

};

#endif //CITYOFWEIRDFISHES_SEVALUATORMAIN_H

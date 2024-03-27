//
// Created by ganymede on 3/21/24.
//

#ifndef CITYOFWEIRDFISHES_SMAINEVALUATOR_H
#define CITYOFWEIRDFISHES_SMAINEVALUATOR_H

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "../../../common/RelPath.h"
#include "items/sRollingListsEvaluator.h"
#include "items/sTotalElements.h"

class sMainEvaluator {
public:
    sMainEvaluator() {
        sME_totalElements = std::make_shared<sTotalElements>("FObjProduced.csv");
        sME_gBasicPrices = std::vector<uint32_t>(sME_totalElements->nElements(), 10);
        sME_vLastTransactions = std::vector<sRollingListsEvaluator>(sME_totalElements->nElements(),
                                                                    sRollingListsEvaluator(5));
        sME_companyHasItem = std::vector<std::list<std::shared_ptr<objCompany>>>(sME_totalElements->nElements(),
                                                                                 std::list<std::shared_ptr<objCompany>>());
    };


    void tickReduced(uint32_t tReduced, uint32_t tDate) {
        if (tReduced == 0)
            updateStaticElements();
    }

    void computeCreatedElement(uint64_t uuidElement, const std::shared_ptr<objCompany> &objAction) {
        sME_gAvailableItems[uuidElement] += 1;
        sME_vLastTransactions[uuidElement].addLastCreate();

        addElementCompany(uuidElement, 1, objAction);
        sME_companyHasItem[uuidElement].push_back(objAction);
    }

    void computeBoughtElement(uint64_t uuidElement, const std::shared_ptr<objCompany> &objAction) {
        uint32_t pItem = getPriceItemActual(uuidElement);
        sME_vLastTransactions[uuidElement].addLastBought();

        if (sME_gAvailableItems.find(uuidElement) != sME_gAvailableItems.end() &&
            sME_gAvailableItems[uuidElement] > 0) {

            removeElementCompany(uuidElement, 1, sME_companyHasItem[uuidElement].front());
            if (sME_companyHasItem[uuidElement].front()->c_pOwn[uuidElement] <= 0)
                sME_companyHasItem[uuidElement].pop_front();
            sME_gAvailableItems[uuidElement] -= 1;
        }

        addElementCompany(uuidElement, 1, objAction);
        objAction->c_cActiveFunds -= pItem;
    }

    void computeSellInmElement(uint64_t uuidElement, const std::shared_ptr<objCompany> &objAction) {
        uint32_t pItem = getPriceItemActual(uuidElement);

        removeElementCompany(uuidElement, 1, objAction);
        objAction->c_cActiveFunds += pItem;
    }

    sTotalElements::sME_Element getById(uint64_t uuidElement) { return sME_totalElements->getById(uuidElement); }

private:

    static void
    addElementCompany(uint64_t uuidElement, uint8_t nElements, const std::shared_ptr<objCompany> &objAction) {
        if (objAction->c_pOwn.find(uuidElement) == objAction->c_pOwn.end())
            objAction->c_pOwn[uuidElement] = nElements; //TO IMPROVE, potser fer-ne mes de un... cada cop
        else
            objAction->c_pOwn[uuidElement] += nElements;
    }

    static void
    removeElementCompany(uint64_t uuidElement, uint8_t nElements, const std::shared_ptr<objCompany> &objAction) {
        if (objAction->c_pOwn.find(uuidElement) == objAction->c_pOwn.end())
            objAction->c_pOwn[uuidElement] = 0;// TO IMPROVE, potser fer-ne mes de un... cada cop
        else
            objAction->c_pOwn[uuidElement] -= nElements;
    }

    uint32_t getPriceItemActual(uint64_t uuidElement) {
        if (sME_gAvailableItems[uuidElement] <= 0)
            return (uint32_t) (1.1 * sME_totalElements->getById(uuidElement).getPrice(
                    sME_vLastTransactions[uuidElement].getDesirability(), sME_gBasicPrices));
        return sME_totalElements->getById(uuidElement).getPrice(
                sME_vLastTransactions[uuidElement].getDesirability(), sME_gBasicPrices);
    }

    void updateStaticElements() {
        //They are update only for the information that the falled window of rolling proportions us.
        for (int i = 0; i < sME_gBasicPrices.size(); i++)
            sME_gBasicPrices[i] = uint32_t(sME_gBasicPrices[i] * sME_vLastTransactions[i].dropLastWindow());
    }

    std::shared_ptr<sTotalElements> sME_totalElements;

    std::vector<uint32_t> sME_gBasicPrices; // Updated once a day;
    std::vector<sRollingListsEvaluator> sME_vLastTransactions;

    std::map<std::string, uint32_t> sME_gItemUuidByName;
    std::map<uint64_t, uint32_t> sME_gAvailableItems;

    std::vector<std::list<std::shared_ptr<objCompany>>> sME_companyHasItem;

};

#endif //CITYOFWEIRDFISHES_SMAINEVALUATOR_H

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
    sMainEvaluator(){
        sME_totalElements = std::make_shared<sTotalElements>("FObjProduced.csv");
        sME_gBasicPrices = std::vector<uint32_t>(sME_totalElements->nElements(),10);
        sME_vLastTransactions = std::vector<sRollingListsEvaluator>(sME_totalElements->nElements(),sRollingListsEvaluator(5));
    };

    void addCreatedElement(uint64_t uuidElement) {
        sME_gAvailableItems[uuidElement] += 1;
        sME_vLastTransactions[uuidElement].addLastCreate();
    }

    void addBoughtElement(uint64_t uuidElement) {
        sME_gAvailableItems[uuidElement] += 1;
        sME_vLastTransactions[uuidElement].addLastBought();
    }

    uint32_t getPriceItemActual(uint64_t uuidElement) {
        if (sME_gAvailableItems[uuidElement] > 0)
            return 1.1 * sME_totalElements->getById(uuidElement).getPrice(
                    sME_vLastTransactions[uuidElement].getDesirability(), sME_gBasicPrices);
        return sME_totalElements->getById(uuidElement).getPrice(
                sME_vLastTransactions[uuidElement].getDesirability(), sME_gBasicPrices);
    }

    sTotalElements::sME_Element getById(uint64_t uuidElement){return sME_totalElements->getById(uuidElement);}
private:

    void updateStaticElements() {
        //They are update only for the information that the falled window of rolling proportions us.
        for (int i = 0; i < sME_gBasicPrices.size(); i++)
            sME_gBasicPrices[i] = uint32_t(sME_gBasicPrices[i] * sME_vLastTransactions[i].dropLastWindow());
    }

    std::shared_ptr<sTotalElements> sME_totalElements;

    std::vector<uint32_t> sME_gBasicPrices; // Updated once a day;
    std::vector<sRollingListsEvaluator> sME_vLastTransactions; // Updated once a day;

    std::map<std::string, uint32_t> sME_gItemUuidByName;
    std::map<uint64_t, uint32_t> sME_gAvailableItems;

    uint32_t sMe_premiumValued = 12;
    uint32_t sME_premiumImport = 20;
};

#endif //CITYOFWEIRDFISHES_SMAINEVALUATOR_H

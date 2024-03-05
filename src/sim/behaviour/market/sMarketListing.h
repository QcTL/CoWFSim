//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SMARKETLISTING_H
#define CITYOFWEIRDFISHES_SMARKETLISTING_H

#include <utility>
#include <cstdint>
#include <list>
#include <algorithm>
#include <memory>
#include <cmath>

#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"

class sMarketListing {
public:
    struct sMOffering {
        std::pair<int, int> sMO_pos;
        uint16_t sMO_quality;
        uint32_t sMO_askingPrice;
    };

    void addListing(const sMOffering &sMO) {
        sMList.insert(std::find_if(sMList.begin(), sMList.end(),
                                   [&](const sMOffering &item) { return sMO.sMO_askingPrice >= item.sMO_askingPrice; }
        ), sMO);
    }

    void shuffleListing(){
        if (!sMList.empty()) {
            sMOffering lBottomRank = sMList.back();
            sMList.pop_back();
            lBottomRank.sMO_askingPrice *=   static_cast<uint32_t>(std::floor(lBottomRank.sMO_askingPrice * 0.95));
            addListing(lBottomRank);
        }
    }

    sMOffering getOfferingByType(const uint8_t typeBuild, const std::shared_ptr<gIGrid<uint8_t>> &gTable) {
        std::list<sMOffering> typeCorrectList;
        std::copy_if(sMList.begin(), sMList.end(), std::back_inserter(typeCorrectList),
                     [&gTable](sMOffering x) { return gTable->get(x.sMO_pos); });

        auto maxIterator = std::max_element(typeCorrectList.begin(), typeCorrectList.end(),
                                            [](const sMOffering &a, const sMOffering &b) {
                                                return a.sMO_quality / a.sMO_askingPrice <
                                                       b.sMO_quality / b.sMO_askingPrice;
                                            });
        if (maxIterator != typeCorrectList.end()) {
            sMList.erase(maxIterator);
            return *maxIterator; //POTSER JA S'HA ELIMINAT LA REFERENCIA I AIXO EM PETARA A LA CARA :)
        }
        return {};
    }

private:
    std::list<sMOffering> sMList;
};

#endif //CITYOFWEIRDFISHES_SMARKETLISTING_H

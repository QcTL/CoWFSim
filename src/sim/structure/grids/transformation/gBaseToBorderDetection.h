//
// Created by Laminar on 28/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOBORDERDETECTION_H
#define CITYOFWEIRDFISHES_GBASETOBORDERDETECTION_H

#include <memory>
#include <map>
#include <unordered_set>
#include "../gIGrid.h"
#include <functional>

enum gBorderType {
    gBConnex, gBNonConnex
};

template<typename T>
class gBaseToBorderDetection {
public:
    gBaseToBorderDetection(std::shared_ptr<gIGrid<T>> gR, gBorderType gBType)
            : gGrid(gR), gUsedBorderType(gBType) {
    }

    std::map<T, std::vector<std::pair<std::pair<int, int>, uint8_t>>>
    generate(std::function<bool(int)> gFValidNumberToBorder) {
        std::map<T, std::vector<std::pair<std::pair<int, int>, uint8_t>>> ret;
        std::pair<std::pair<int, int>, std::pair<int, int>> range = gGrid->rangeUse();

        switch (gUsedBorderType) {
            case gBConnex:
                break;
            case gBNonConnex:
                for (int i = range.second.first; i < (range.second.second + 1); i++) {
                    for (int j = range.first.first; j < (range.first.second + 1); j++) {
                        int vToSearch = gGrid->get(j, i);
                        if (gFValidNumberToBorder(vToSearch)) {
                            std::vector<std::pair<int, int>> dOffsets =
                                    {{i - 1, j + 1},
                                     {i,     j + 1},
                                     {i + 1, j + 1},
                                     {i - 1, j},
                                     {i + 1, j},
                                     {i - 1, j - 1},
                                     {i,     j - 1},
                                     {i + 1, j - 1}};
                            uint8_t vRes = 0;

                            for (int k = 0; k < dOffsets.size(); k++) {
                                if (gGrid->isInside(dOffsets[k].second, dOffsets[k].first) &&
                                    gGrid->get(dOffsets[k].second, dOffsets[k].first) == vToSearch) {
                                    vRes |= uint8_t(1) << k;
                                }
                            }
                            ret[vToSearch].push_back({{i, j}, vRes});
                        }
                    }
                }
                break;
        }
        return ret;
    }

private:
    std::shared_ptr<gIGrid<T>> gGrid;
    gBorderType gUsedBorderType;
    std::unordered_set<int> gBorderedValues;
};

#endif //CITYOFWEIRDFISHES_GBASETOBORDERDETECTION_H

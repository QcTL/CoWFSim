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

enum gBorderOutside {
    gIsGroup, gIsNotGroup
};

struct gObjectBorderDet {
    gBorderType gBT;
    gBorderOutside gBO;
};


class gBaseToBorderDetection {
public:

    template<typename T>
    static std::map<T, std::vector<std::pair<std::pair<int, int>, uint8_t>>>
    generate(const std::shared_ptr<gIGrid<T>> &gGrid, gObjectBorderDet gOBD,
             std::vector<T> groupConnect = std::vector<T>(), const std::shared_ptr<gIGrid<bool>> &pMask = nullptr) {

        bool hasMask = pMask != nullptr;
        bool hasDiffGroup = !groupConnect.empty();
        std::unordered_set<int> setGroup(groupConnect.begin(), groupConnect.end());

        std::map<T, std::vector<std::pair<std::pair<int, int>, uint8_t>>> ret;
        std::pair<std::pair<int, int>, std::pair<int, int>> range = gGrid->rangeUse();

        switch (gOBD.gBT) {
            case gBConnex:
                break;
            case gBNonConnex:
                for (int i = range.second.first; i < (range.second.second + 1); i++) {
                    for (int j = range.first.first; j < (range.first.second + 1); j++) {
                        T vToSearch = gGrid->get(j, i);
                        if (!hasMask || pMask->get(j, i)) {
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
                                if (gGrid->isInside(dOffsets[k].second, dOffsets[k].first)) {
                                    T valNext = gGrid->get(dOffsets[k].second, dOffsets[k].first);

                                    if ((!hasMask || pMask->get(dOffsets[k].second, dOffsets[k].first)) &&
                                        (valNext == vToSearch ||
                                         (hasDiffGroup && setGroup.find(valNext) != setGroup.end()))) {
                                        vRes |= uint8_t(1) << k;
                                    }
                                } else if (gOBD.gBO == gBorderOutside::gIsGroup &&
                                           !gGrid->isInside(dOffsets[k].second, dOffsets[k].first)) {
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


    template<typename T>
    static std::map<T, std::vector<std::pair<std::pair<int, int>, uint16_t>>>
    generateByTwoBits(const std::shared_ptr<gIGrid<T>> &gGrid, gObjectBorderDet gOBD,
                      std::vector<T> groupConnect = std::vector<T>(),
                      const std::shared_ptr<gIGrid<bool>> &pMask = nullptr) {

        bool hasMask = pMask != nullptr;
        bool hasDiffGroup = !groupConnect.empty();
        std::unordered_set<int> setGroup(groupConnect.begin(), groupConnect.end());

        std::map<T, std::vector<std::pair<std::pair<int, int>, uint16_t>>> ret;
        std::pair<std::pair<int, int>, std::pair<int, int>> range = gGrid->rangeUse();

        switch (gOBD.gBT) {
            case gBConnex:
                break;
            case gBNonConnex:
                for (int i = range.second.first; i < (range.second.second + 1); i++) {
                    for (int j = range.first.first; j < (range.first.second + 1); j++) {
                        T vToSearch = gGrid->get(j, i);
                        if (!hasMask || pMask->get(j, i)) {
                            std::vector<std::pair<int, int>> dOffsets =
                                    {{i - 1, j + 1},
                                     {i,     j + 1},
                                     {i + 1, j + 1},
                                     {i - 1, j},
                                     {i + 1, j},
                                     {i - 1, j - 1},
                                     {i,     j - 1},
                                     {i + 1, j - 1}};
                            uint16_t vRes = 0;

                            for (int k = 0; k < dOffsets.size(); k++) {
                                if (gGrid->isInside(dOffsets[k].second, dOffsets[k].first)) {
                                    T valNext = gGrid->get(dOffsets[k].second, dOffsets[k].first);

                                    if ((!hasMask || pMask->get(dOffsets[k].second, dOffsets[k].first)) &&
                                        (valNext == vToSearch ||
                                         (hasDiffGroup && setGroup.find(valNext) != setGroup.end())))
                                        vRes |= (valNext == 553648128 ? 2 : 1) << k * 2;
                                } else if (gOBD.gBO == gBorderOutside::gIsGroup)
                                    vRes |= (vToSearch == 553648128 ? 2 : 1) << k * 2;
                            }
                            ret[vToSearch].push_back({{i, j}, vRes});
                        }
                    }
                }
                break;
        }
        return ret;
    }
};

#endif //CITYOFWEIRDFISHES_GBASETOBORDERDETECTION_H

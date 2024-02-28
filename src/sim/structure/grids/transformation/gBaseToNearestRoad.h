//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETONEARESTROAD_H
#define CITYOFWEIRDFISHES_GBASETONEARESTROAD_H

#include <memory>
#include <numeric>
#include "../../../roads/node/rNode.h"

class gBaseToNearestRoad {
public:
    static void
    givenMatRef(const std::shared_ptr<gIGrid<rNode *>> &rGrid, const std::vector<std::vector<rNode *>> &vectInf,
                const std::shared_ptr<gIGrid<uint8_t>> &genType) {
        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = rGrid->rangeUse();
        for (int i = gRange.first.first; i <= gRange.first.second; i++) {
            for (int j = gRange.second.first; j <= gRange.second.second; j++) {
                if (genType->get(i, j) == 1) {
                    uint8_t dDist = 1;
                    rNode *nearRoad = nullptr;
                    while (dDist < 16 && nearRoad == nullptr) {
                        if (dDist % 2 != 0) {
                            int gValid = (dDist + 1) / 2;
                            if (genType->isInside(i + gValid, j) && genType->get(i + gValid, j) == 2)
                                nearRoad = vectInf[i + gValid][j];
                            else if (genType->isInside(i - gValid, j) && genType->get(i - gValid, j) == 2)
                                nearRoad = vectInf[i - gValid][j];
                            else if (genType->isInside(i, j + gValid) && genType->get(i, j + gValid) == 2)
                                nearRoad = vectInf[i][j + gValid];
                            else if (genType->isInside(i, j - gValid) && genType->get(i, j - gValid) == 2)
                                nearRoad = vectInf[i][j - gValid];
                        } else {
                            int gValid = dDist / 2;
                            std::vector<int> vec(gValid);
                            std::vector<int> vecRev(gValid);

                            std::iota(vec.begin(), vec.end(), 1);
                            std::iota(vecRev.rbegin(), vecRev.rend(), 1);
                            for (int k = 0; k < vec.size(); k++) {
                                if (genType->get(i + vec[k], j + vecRev[k]) == 2)
                                    nearRoad = vectInf[i + vec[k]][j + vecRev[k]];
                                if (genType->get(i - vec[k], j + vecRev[k]) == 2)
                                    nearRoad = vectInf[i - vec[k]][j + vecRev[k]];
                                if (genType->get(i + vec[k], j - vecRev[k]) == 2)
                                    nearRoad = vectInf[i + vec[k]][j - vecRev[k]];
                                if (genType->get(i - vec[k], j - vecRev[k]) == 2)
                                    nearRoad = vectInf[i - vec[k]][j - vecRev[k]];
                            }
                        }
                        dDist++;
                    }
                    rGrid->set(i, j, nearRoad);
                }
            }
        }
    }

};

#endif //CITYOFWEIRDFISHES_GBASETONEARESTROAD_H

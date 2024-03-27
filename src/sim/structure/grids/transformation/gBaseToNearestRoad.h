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
                const std::shared_ptr<gIGrid<uint8_t>> &genType, const std::vector<uint8_t> &gTypesRoad,
                const std::vector<uint8_t> &gTypesBuilding) {


        std::unordered_set<uint8_t> roadGroup(gTypesRoad.begin(), gTypesRoad.end());
        std::unordered_set<uint8_t> buildingGroup(gTypesBuilding.begin(), gTypesBuilding.end());

        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = rGrid->rangeUse();
        for (int i = gRange.second.first; i < (gRange.second.second + 1); i++) {
            for (int j = gRange.first.first; j < (gRange.first.second + 1); j++) {
                if (buildingGroup.find(genType->get(i, j)) != buildingGroup.end()) {
                    uint8_t dDist = 1;
                    rNode *nearRoad = nullptr;
                    while (dDist < 16 && nearRoad == nullptr) {
                        if (dDist % 2 != 0) {
                            int gValid = (dDist + 1) / 2;
                            if (genType->isInside(i + gValid, j) &&
                                isInsideGroup(roadGroup, genType->get(i + gValid, j)))
                                nearRoad = vectInf[i + gValid][j];
                            else if (genType->isInside(i - gValid, j) &&
                                     isInsideGroup(roadGroup, genType->get(i - gValid, j)))
                                nearRoad = vectInf[i - gValid][j];
                            else if (genType->isInside(i, j + gValid) &&
                                     isInsideGroup(roadGroup, genType->get(i, j + gValid)))
                                nearRoad = vectInf[i][j + gValid];
                            else if (genType->isInside(i, j - gValid) &&
                                     isInsideGroup(roadGroup, genType->get(i, j - gValid)))
                                nearRoad = vectInf[i][j - gValid];
                        } else {
                            int gValid = dDist / 2;
                            std::vector<int> vec(gValid);
                            std::vector<int> vecRev(gValid);

                            std::iota(vec.begin(), vec.end(), 1);
                            std::iota(vecRev.rbegin(), vecRev.rend(), 1);
                            for (int k = 0; k < vec.size(); k++) {
                                if (isInsideGroup(roadGroup, genType->get(i + vec[k], j + vecRev[k])))
                                    nearRoad = vectInf[i + vec[k]][j + vecRev[k]];
                                if (isInsideGroup(roadGroup, genType->get(i - vec[k], j + vecRev[k])))
                                    nearRoad = vectInf[i - vec[k]][j + vecRev[k]];
                                if (isInsideGroup(roadGroup, genType->get(i + vec[k], j - vecRev[k])))
                                    nearRoad = vectInf[i + vec[k]][j - vecRev[k]];
                                if (isInsideGroup(roadGroup, genType->get(i - vec[k], j - vecRev[k])))
                                    nearRoad = vectInf[i - vec[k]][j - vecRev[k]];
                            }
                        }
                        dDist++;
                    }
                    uint8_t r = genType->get(i, j);
                    rGrid->set(i, j, nearRoad);
                }
            }
        }
    }

private:

    bool static isInsideGroup(const std::unordered_set<uint8_t> &g, uint8_t v) {
        return g.find(v) != g.end();
    }
};

#endif //CITYOFWEIRDFISHES_GBASETONEARESTROAD_H

//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETONEARESTROAD_H
#define CITYOFWEIRDFISHES_GBASETONEARESTROAD_H

#include <memory>
#include <numeric>
#include "../../../roads/node/rNode.h"
#include "../../../groups/groupLand/sgTerrain.h"

class gBaseToNearestRoad {
public:
    static void
    givenMatRef(const std::shared_ptr<gIGrid<rNode *>> &rGrid, const std::vector<std::vector<rNode *>> &vecInf,
                const std::shared_ptr<sgTerrain> &inGTerrain,
                const std::vector<uint8_t> &gTypesRoad,
                const std::vector<uint8_t> &gTypesSoil) {


        std::unordered_set<uint8_t> roadGroup(gTypesRoad.begin(), gTypesRoad.end());
        std::unordered_set<uint8_t> soilGroup(gTypesSoil.begin(), gTypesSoil.end());

        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = rGrid->rangeUse();
        for (int i = gRange.second.first; i < (gRange.second.second + 1); i++) {
            for (int j = gRange.first.first; j < (gRange.first.second + 1); j++) {
                if (soilGroup.find(inGTerrain->gTG_TypeSoil->get(i, j)) != soilGroup.end()) {
                    uint8_t dDist = 1;
                    rNode *nearRoad = nullptr;
                    while (dDist < 16 && nearRoad == nullptr) {
                        if (dDist % 2 != 0) {
                            int gValid = (dDist + 1) / 2;
                            if (inGTerrain->gTG_TypeGen->isInside(i + gValid, j) &&
                                isInsideGroup(roadGroup, inGTerrain->gTG_TypeGen->get(i + gValid, j)))
                                nearRoad = vecInf[i + gValid][j];
                            else if (inGTerrain->gTG_TypeGen->isInside(i - gValid, j) &&
                                     isInsideGroup(roadGroup, inGTerrain->gTG_TypeGen->get(i - gValid, j)))
                                nearRoad = vecInf[i - gValid][j];
                            else if (inGTerrain->gTG_TypeGen->isInside(i, j + gValid) &&
                                     isInsideGroup(roadGroup, inGTerrain->gTG_TypeGen->get(i, j + gValid)))
                                nearRoad = vecInf[i][j + gValid];
                            else if (inGTerrain->gTG_TypeGen->isInside(i, j - gValid) &&
                                     isInsideGroup(roadGroup, inGTerrain->gTG_TypeGen->get(i, j - gValid)))
                                nearRoad = vecInf[i][j - gValid];
                        } else {
                            int gValid = dDist / 2;
                            std::vector<int> vec(gValid);
                            std::vector<int> vecRev(gValid);

                            std::iota(vec.begin(), vec.end(), 1);
                            std::iota(vecRev.rbegin(), vecRev.rend(), 1);
                            for (int k = 0; k < vec.size(); k++) {
                                if (inGTerrain->gTG_TypeGen->isInside(i + vec[k], j + vecRev[k]) &&
                                    isInsideGroup(roadGroup, inGTerrain->gTG_TypeGen->get(i + vec[k], j + vecRev[k])))
                                    nearRoad = vecInf[i + vec[k]][j + vecRev[k]];
                                if (inGTerrain->gTG_TypeGen->isInside(i - vec[k], j + vecRev[k]) &&
                                    isInsideGroup(roadGroup, inGTerrain->gTG_TypeGen->get(i - vec[k], j + vecRev[k])))
                                    nearRoad = vecInf[i - vec[k]][j + vecRev[k]];
                                if (inGTerrain->gTG_TypeGen->isInside(i + vec[k], j - vecRev[k]) &&
                                    isInsideGroup(roadGroup, inGTerrain->gTG_TypeGen->get(i + vec[k], j - vecRev[k])))
                                    nearRoad = vecInf[i + vec[k]][j - vecRev[k]];
                                if (inGTerrain->gTG_TypeGen->isInside(i - vec[k], j - vecRev[k]) &&
                                    isInsideGroup(roadGroup, inGTerrain->gTG_TypeGen->get(i - vec[k], j - vecRev[k])))
                                    nearRoad = vecInf[i - vec[k]][j - vecRev[k]];
                            }
                        }
                        dDist++;
                    }
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

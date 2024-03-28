//
// Created by ganymede on 3/21/24.
//

#ifndef CITYOFWEIRDFISHES_SGROADSMAIN_H
#define CITYOFWEIRDFISHES_SGROADSMAIN_H

#include <vector>
#include <memory>
#include "node/rNode.h"
#include "../structure/obj/sRoutesStorage.h"
#include "../structure/grids/gIGrid.h"
#include "../structure/grids/gBasicGrid.h"
#include "rNodeFromGrid.h"

class sgRoadsMain {
public:

    sgRoadsMain(uint32_t inGridSize, const std::shared_ptr<sgTerrain> &inPsgTerrain)
            : sgRM_inGridSize(inGridSize), sgRM_gTerrainGrid(inPsgTerrain) {
        gLayerTransit = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(inGridSize, inGridSize, 0));
        sgRM_gNearRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(inGridSize, inGridSize, nullptr));
    }

    void tickReduced(uint32_t inRTime, uint32_t inTDate) {
        routeCarsCommute(inRTime, inTDate);
        routeCarsGhost(inRTime);
    }

    void tick() {
        for (const std::shared_ptr<rRNodeI> &node: sgRM_listRRoads) {
            node->sendInformationStart();
            node->tick();
            node->sendNewInformation();
        }
    }

    [[nodiscard]] std::pair<int, int> getClosestRoadToBuilding(const std::pair<int, int> &inGridPos) const {
        return sgRM_gNearRoad->get(inGridPos)->rPos;
    }

    [[nodiscard]] std::pair<int, int> getRandomCivilStartRoad() const {
        return sgRM_gNearRoad->get(sgRM_gTerrainGrid->returnRandomFullCivil())->rPos;
    }

    std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator>
    addRuteCivil(const std::shared_ptr<objCivil> &oC) {
        return sgRM_sTRoutes->addRuteCivil(oC);
    }

    void removeRuteCivil(const std::shared_ptr<objCivil> &inObjCivil,
                         const std::list<objCivil>::iterator& gPBegin,
                         const std::list<objCivil>::iterator& gPEnd){
        sgRM_sTRoutes->removeRuteCivil(inObjCivil, gPBegin, gPEnd);
    }


    void completedStartGrid(const std::shared_ptr<sgTerrain> &gMainTerrain) {
        extractRoadsFromLayer(gMainTerrain);
        for (int i = 0; i < 2000; i++) {
            for (const std::shared_ptr<rRNodeI> &node: sgRM_listRRoads) {
                node->sendInformationStart();
                node->sendNewInformation();
            }
        }
        rInfoDist::seeMatrix();
    }

    std::shared_ptr<gIGrid<uint8_t>> gLayerTransit;
    std::vector<std::vector<rNode *>> gLayerRoads;
    std::shared_ptr<sgTerrain> sgRM_gTerrainGrid;
private:

    void routeCarsCommute(const uint32_t inRTime,const uint32_t inTDate) {
        auto newRoutes = sgRM_sTRoutes->getRoutesCarByTime(inRTime, inTDate);
        for (auto r: newRoutes) {
            uint32_t locId = gLayerRoads[r.c_REnd.first][r.c_REnd.second]->refCompressed->locIdNode;
            uint16_t blocId = gLayerRoads[r.c_REnd.first][r.c_REnd.second]->refCompressed->rBlock;
            gLayerRoads[r.c_RStart.first][r.c_RStart.second]->refCompressed->addNewCar(locId, blocId);
        }
    }

    void routeCarsGhost(const uint32_t inRTime) {
        if (inRTime % 24 != 0)
            return;
        for (int i = 0; i < sgRM_vecFreqGhostRiders[inRTime / 24] * 3; i++) {
            std::pair<int, int> _gPointStart = getClosestRoadToBuilding(sgRM_gTerrainGrid->returnRandomFullCivil());
            std::pair<int, int> _gPointEnd = getClosestRoadToBuilding(sgRM_gTerrainGrid->returnRandomFullCivil());
            uint32_t _locId = gLayerRoads[_gPointEnd.first][_gPointEnd.second]->refCompressed->locIdNode;
            uint16_t _blocId = gLayerRoads[_gPointEnd.first][_gPointEnd.second]->refCompressed->rBlock;
            gLayerRoads[_gPointStart.first][_gPointStart.second]->refCompressed->addNewCar(_locId, _blocId);
        }
    }

    void extractRoadsFromLayer(const std::shared_ptr<sgTerrain> &gMainTerrain) {
        std::pair<std::vector<rNode *>, std::vector<std::vector<rNode *>>> r = rNodeFromGrid<uint8_t>::givenGrid(
                gMainTerrain->gTG_TypeGen, {5, 6});
        gLayerRoads = r.second;
        std::cout << r.first.size() << std::endl;
        for (const auto &rNode: r.first) {
            sgRM_listRRoads.merge(rTransRNodeToRRNode().conversion(rNode, sqrt(sgRM_inGridSize), sgRM_inGridSize, sgRM_gTerrainGrid->gTG_TypeGen,
                                                                   gLayerTransit));
            rInfoDist::initializeMatrix(sgRM_inGridSize / sqrt(sgRM_inGridSize) * sgRM_inGridSize / sqrt(sgRM_inGridSize), sgRM_inGridSize, sgRM_listRRoads.size());
        }

        gBaseToNearestRoad::givenMatRef(sgRM_gNearRoad, gLayerRoads, gMainTerrain->gTG_TypeGen, {5, 6}, {1, 2, 3, 4});

        for (uint32_t i = 0; i < r.second.size(); ++i) {
            for (uint32_t j = 0; j < r.second[i].size(); ++j) {
                if (r.second[i][j] != nullptr) {
                    gLayerTransit->set(i, j, 1);
                    sgRM_vecRNodes.push_back(r.second[i][j]);
                }
            }
        }
    };

    std::list<std::shared_ptr<rRNodeI>> sgRM_listRRoads;

    uint32_t sgRM_inGridSize;

    std::shared_ptr<sRoutesStorage> sgRM_sTRoutes = std::make_shared<sRoutesStorage>();
    std::shared_ptr<gIGrid<rNode *>> sgRM_gNearRoad;

    std::vector<rNode *> sgRM_vecRNodes;
    std::vector<uint8_t> sgRM_vecFreqGhostRiders = {1, 1, 2, 3, 5, 4, 3, 4, 5, 4, 3, 2, 2, 1};
};

#endif //CITYOFWEIRDFISHES_SGROADSMAIN_H

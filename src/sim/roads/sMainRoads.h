//
// Created by ganymede on 3/21/24.
//

#ifndef CITYOFWEIRDFISHES_SMAINROADS_H
#define CITYOFWEIRDFISHES_SMAINROADS_H

#include <vector>
#include <memory>
#include "node/rNode.h"
#include "../structure/obj/sTotalRoutes.h"
#include "../structure/grids/gIGrid.h"
#include "../structure/grids/gBasicGrid.h"
#include "rNodeFromGrid.h"

class sMainRoads {
public:

    sMainRoads(uint32_t lSize, const std::shared_ptr<gTerrainGrid> &gTerrainGrid) : sizeL(lSize),
                                                                                    sMR_gTerrainGrid(gTerrainGrid) {
        gLayerTransit = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        gLayerNextRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(lSize, lSize, nullptr));
    }

    void tickReduced(uint32_t tReduced, uint32_t tDate) {
        auto newRoutes = sTCivil->getRoutesCarByTime(tReduced, tDate);
        for (auto r: newRoutes) {
            uint32_t locId = gLayerRoads[r.c_REnd.first][r.c_REnd.second]->refCompressed->locIdNode;
            uint16_t blocId = gLayerRoads[r.c_REnd.first][r.c_REnd.second]->refCompressed->rBlock;
            gLayerRoads[r.c_RStart.first][r.c_RStart.second]->refCompressed->addNewCar(locId, blocId);
        }

        if (tReduced % 24 == 0) {
            //Maybe add between two random points based on a distribution;
            for (int i = 0; i < gVecFreqGhostRiders[tReduced / 24]*3; i++) {
                std::pair<int, int> gPointStart = getClosestRoadToBuilding(sMR_gTerrainGrid->returnRandomFullCivil());
                std::pair<int, int> gPointEnd = getClosestRoadToBuilding(sMR_gTerrainGrid->returnRandomFullCivil());
                std::cout << gPointStart.first << "-" << gPointStart.second << "   <>    " << gPointEnd.first << "-"
                          << gPointEnd.second << std::endl;
                uint32_t locId = gLayerRoads[gPointEnd.first][gPointEnd.second]->refCompressed->locIdNode;
                uint16_t blocId = gLayerRoads[gPointEnd.first][gPointEnd.second]->refCompressed->rBlock;
                gLayerRoads[gPointStart.first][gPointStart.second]->refCompressed->addNewCar(locId, blocId);
            }
        }
    }

    void tick() {
        for (const std::shared_ptr<rRNodeI> &node: rListRRoads) {
            node->sendInformationStart();
            node->tick();
            node->sendNewInformation();
        }
    }

    [[nodiscard]] std::pair<int, int> getClosestRoadToBuilding(const std::pair<int, int> &gPos) const {
        return gLayerNextRoad->get(gPos)->rPos;
    }

    [[nodiscard]] std::pair<int, int> getRandomCivilStartRoad() const {
        return gLayerNextRoad->get(sMR_gTerrainGrid->returnRandomFullCivil())->rPos;
    }

    std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator>
    addRuteCivil(const std::shared_ptr<objCivil> &oC) {
        return sTCivil->addRuteCivil(oC);
    }

    void completedStartGrid(const std::shared_ptr<gTerrainGrid> &gMainTerrain) {
        extractRoadsFromLayer(gMainTerrain);
        for (int i = 0; i < 2000; i++) {
            for (const std::shared_ptr<rRNodeI> &node: rListRRoads) {
                node->sendInformationStart();
                node->sendNewInformation();
            }
        }
        rInfoDist::seeMatrix();
    }

    std::shared_ptr<gIGrid<uint8_t>> gLayerTransit;
    std::vector<std::vector<rNode *>> gLayerRoads;
    std::shared_ptr<gTerrainGrid> sMR_gTerrainGrid;
private:
    void extractRoadsFromLayer(const std::shared_ptr<gTerrainGrid> &gMainTerrain) {
        std::pair<std::vector<rNode *>, std::vector<std::vector<rNode *>>> r = rNodeFromGrid<uint8_t>::givenGrid(
                gMainTerrain->gTG_TypeGen, {5, 6});
        gLayerRoads = r.second;
        std::cout << r.first.size() << std::endl;
        for (const auto &rNode: r.first) {
            rListRRoads.merge(rTransRNodeToRRNode().conversion(rNode, sqrt(sizeL), sizeL, gLayerTransit));
            rInfoDist::initializeMatrix(sizeL / sqrt(sizeL) * sizeL / sqrt(sizeL), sizeL, rListRRoads.size());
        }

        gBaseToNearestRoad::givenMatRef(gLayerNextRoad, gLayerRoads, gMainTerrain->gTG_TypeGen, {5, 6}, {1, 2, 3, 4});

        for (uint32_t i = 0; i < r.second.size(); ++i) {
            for (uint32_t j = 0; j < r.second[i].size(); ++j) {
                if (r.second[i][j] != nullptr) {
                    gLayerTransit->set(i, j, 1);
                    gTotalNodes.push_back(r.second[i][j]);
                }
            }
        }

        //TEST DAILY COMMUTE:
        /*
        for (int i = 0; i < 10; i++) {
            uint32_t tTime = i;
            uint32_t tTimeEnd = i + 48;

            for (int j = 0; j < 3; j++) {
                int idP1;
                int idP2;
                choseFromVector(gTotalNodes.size(), idP1, idP2);

                sTCivil->addRuteCivil({{gTotalNodes[idP1]->rPos, gTotalNodes[idP2]->rPos}, tTime, tTimeEnd});
            }
        }*/
        //sTCivil->addRuteCivil({{{0, 0}, {4, 2}}, 5, 250});
    };
    std::list<std::shared_ptr<rRNodeI>> rListRRoads;

    uint32_t sizeL;

    std::shared_ptr<sTotalRoutes> sTCivil = std::make_shared<sTotalRoutes>();
    std::vector<rNode *> gTotalNodes;
    std::shared_ptr<gIGrid<rNode *>> gLayerNextRoad;

    std::vector<uint8_t> gVecFreqGhostRiders = {1, 1, 2, 3, 5, 4, 3, 4, 5, 4, 3, 2, 2, 1};
};

#endif //CITYOFWEIRDFISHES_SMAINROADS_H

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
#include "../groups/groupLand/groupLand.h"
#include "../structure/grids/transformation/gBaseToNearestRoad.h"
#include "rTransRNodeToRRNode.h"
#include "../events/sEventManager.h"

class sgRoadsMain {
public:

    sgRoadsMain(uint32_t inGridSize, const std::shared_ptr<groupLand> &inGLand)
            : sgRM_inGridSize(inGridSize), sgRM_gLand(inGLand) {
        gLayerTransit = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(inGridSize, inGridSize, 0));
        sgRM_gNearRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(inGridSize, inGridSize, nullptr));
    }
    /**
     * @fn void tickReduced
     * @brief Updates the elements in the class that need changing every 5 tick of the simulation
     * @param inRTime uint representing the reduced time of the day
     * @param inTDate uint representing the reduced date of the simulation
     */
    void tickReduced(uint32_t inRTime, uint32_t inTDate) {
        routeCarsCommute(inRTime, inTDate);
        routeCarsGhost(inRTime);
    }

    /**
     * @fn void tick
     * @brief Updates the elements in the class that need changing every tick of the simulation
     */
    void tick() {
        for (const std::shared_ptr<rRNodeI> &node: sgRM_listRRoads) {
            node->sendInformationStart();
            node->tick();
            node->sendNewInformation();
        }
    }

    /**
     * @fn std::pair<int, int> getClosestRoadToBuilding
     * @brief Given a position on the matrix return the closes position that has a road in it.
     * @param inGridPos The position you want the closes road to
     * @return The coordinate of the closes road
     */
    [[nodiscard]] std::pair<int, int> getClosestRoadToBuilding(const std::pair<int, int> &inGridPos) const {
        return sgRM_gNearRoad->get(inGridPos)->rPos;
    }

    [[nodiscard]] std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator>
    addRuteCivil(const std::shared_ptr<objCivil> &oC) const {
        return sgRM_sTRoutes->addRuteCivil(oC);
    }

    /**
     * @fn void removeRuteCivil
     * @brief Given the shared pointer of the civil you want the remove and its position given the iterators,
     * remove the route from the active ones.
     * @param inObjCivil Given the shared pointer of the civil you want to remove the route
     * @param gPBegin The iterator of the position of the obj in the list Begin
     * @param gPEnd The iterator of the position of the obj in the list End
     */
    void removeRuteCivil(const std::shared_ptr<objCivil> &inObjCivil,
                         const std::list<objCivil>::iterator &gPBegin,
                         const std::list<objCivil>::iterator &gPEnd) {
        sgRM_sTRoutes->removeRuteCivil(inObjCivil, gPBegin, gPEnd);
    }

    /**
     * @fn void completedStartGrid
     * @brief This functions makes all the nodes that are connected speak with each-other to learn the routes necessary
     * to reach any other position.
     */
    void completedStartGrid() {
        extractRoadsFromLayer();
        for (int i = 0; i < 2000; i++) {
            for (const std::shared_ptr<rRNodeI> &node: sgRM_listRRoads) {
                node->sendInformationStart();
                node->sendNewInformation();
            }
        }
        rInfoDist::seeMatrix();
    }

    /**
     * @fn std::vector<objActiveRute> getRoutesByType
     * @brief Given the necessary times and the type of route you want it returns the actives routes that should be
     * starting in that moment
     * @param inRouteType A value of the enum objCivil::typeRouteSystem  with the possible type of route
     * @param cTime A uint of the reduced time of the day < 288
     * @param cDate A uint of the reduced date of the simulation, has to comply with the stipulations of reduced date.
     * @return The vector of routes that start in that moment
     */
    std::vector<objActiveRute>
    getRoutesByType(const objCivil::typeRouteSystem inRouteType, uint32_t cTime, uint32_t cDate) {
        return sgRM_sTRoutes->getRoutesByType(inRouteType, cTime, cDate);
    }

    std::shared_ptr<gIGrid<uint8_t>> gLayerTransit;
    std::vector<std::vector<rNode *>> gLayerRoads;
    std::shared_ptr<groupLand> sgRM_gLand;
private:

    /**
     * @fn void routeCarsCommute
     * @brief Given the date and the time, start the routes of all the ones that start in that timeframe
     * @param inRTime A uint of the reduced time of the day < 288
     * @param inTDate  A uint of the reduced date of the simulation, has to comply with the stipulations of reduced date
     */
    void routeCarsCommute(const uint32_t inRTime, const uint32_t inTDate) {
        auto newCarRoutes = sgRM_sTRoutes->getRoutesByType(objCivil::typeRouteSystem::OC_TRS_CAR, inRTime, inTDate);
        for (auto &r: newCarRoutes) {
            sEventManager::getInstance()->callEventStartRoute(inRTime, inTDate, r.c_uuid, r);
            uint32_t locId = gLayerRoads[r.c_REnd.first][r.c_REnd.second]->refCompressed->locIdNode;
            uint16_t blocId = gLayerRoads[r.c_REnd.first][r.c_REnd.second]->refCompressed->rBlock;
            gLayerRoads[r.c_RStart.first][r.c_RStart.second]->refCompressed->addNewCar(locId, blocId);
        }

        auto newMetroRoutes = sgRM_sTRoutes->getRoutesByType(objCivil::typeRouteSystem::OC_TRS_TRAIN, inRTime, inTDate);
        for (auto &r: newMetroRoutes) {
            sEventManager::getInstance()->callEventStartRoute(inRTime, inTDate, r.c_uuid, r);
        }
    }
    /**
     * @fn void routeCarsGhost
     * @brief Given the date and the time, start the routes of ghost cars that start in that timeframe.
     * Ghost cars are the approximation of number of routes that are done outside the daily work commute
     * @param inRTime A uint of the reduced time of the day < 288
     */
    void routeCarsGhost(const uint32_t inRTime) {
        if (inRTime % 24 != 0)
            return;
        for (int i = 0; i < sgRM_vecFreqGhostRiders[inRTime / 24] * 3; i++) {
            std::pair<int, int> _gPointStart = getClosestRoadToBuilding(
                    sgRM_gLand->gL_gTerrain->returnRandomFullCivil());
            std::pair<int, int> _gPointEnd = getClosestRoadToBuilding(
                    sgRM_gLand->gL_gTerrain->returnRandomFullCivil());
            uint32_t _locId = gLayerRoads[_gPointEnd.first][_gPointEnd.second]->refCompressed->locIdNode;
            uint16_t _blocId = gLayerRoads[_gPointEnd.first][_gPointEnd.second]->refCompressed->rBlock;
            gLayerRoads[_gPointStart.first][_gPointStart.second]->refCompressed->addNewCar(_locId, _blocId);
        }
    }

    /**
     * @fn void extractRoadsFromLayer
     * @brief Given the cell matrix where the roads are represented from values, extract them from the world map and make the necessary
     * structures to function them abstractly of the common map.
     */
    void extractRoadsFromLayer() {
        std::pair<std::vector<rNode *>, std::vector<std::vector<rNode *>>> r = rNodeFromGrid<uint8_t>::givenGrid(
                sgRM_gLand->gL_gTerrain->gTG_TypeGen, {5, 6});
        gLayerRoads = r.second;
        std::cout << r.first.size() << std::endl;
        for (const auto &rNode: r.first) {
            sgRM_listRRoads.merge(rTransRNodeToRRNode().conversion(rNode, sqrt(sgRM_inGridSize), sgRM_inGridSize,
                                                                   sgRM_gLand->gL_gTerrain->gTG_TypeGen,
                                                                   gLayerTransit));
            rInfoDist::initializeMatrix(
                    sgRM_inGridSize / sqrt(sgRM_inGridSize) * sgRM_inGridSize / sqrt(sgRM_inGridSize), sgRM_inGridSize,
                    sgRM_listRRoads.size());
        }
        gBaseToNearestRoad::givenMatRef(sgRM_gNearRoad, gLayerRoads, sgRM_gLand->gL_gTerrain,
                                        {
                                                sgTerrain::sgT_TypeGen::sgT_TG_RoadS,
                                                sgTerrain::sgT_TypeGen::sgT_TG_RoadB},
                                        {
                                                sgTerrain::sgT_TypeSoil::sgT_TS_T1Mixed,
                                                sgTerrain::sgT_TypeSoil::sgT_TS_T2Mixed,
                                                sgTerrain::sgT_TypeSoil::sgT_TS_T3Mixed,
                                                sgTerrain::sgT_TypeSoil::sgT_TS_T1Industrial,
                                                sgTerrain::sgT_TypeSoil::sgT_TS_T2Industrial,
                                                sgTerrain::sgT_TypeSoil::sgT_TS_T1Farm});
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

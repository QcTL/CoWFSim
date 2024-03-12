//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SMAINSIMULATOR_H
#define CITYOFWEIRDFISHES_SMAINSIMULATOR_H

#include <cstdint>
#include <memory>
#include <vector>
#include "roads/node/rNode.h"
#include "roads/rTransRNodeToRRNode.h"
#include "roads/rNodeFromGrid.h"
#include "structure/grids/gBasicGrid.h"
#include "structure/grids/gIGrid.h"
#include "structure/obj/sCommon.h"
#include "structure/obj/sTotalCompany.h"
#include "structure/grids/transformation/gBaseToNearestRoad.h"
#include "../display/rPileMenus.h"
#include "structure/obj/sTotalRoutes.h"
#include "behaviour/company/sMCompany.h"
#include "behaviour/airPollution/gMainAirPollution.h"

class sMainSimulator {

public:
    explicit sMainSimulator(int lSize) : sizeL(lSize) {
        gLayerTypeSoil = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        gLayerTypeGen = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        gLayerCurStruct = std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(lSize, lSize, 0));
        gLayerTransit = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        gLayerNextRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(lSize, lSize, nullptr));
        gTotalAirPollution = std::make_shared<gMainAirPollution>(lSize);

        sComp = std::make_shared<sMCompany>(lSize,
                                            gLayerTypeGen,
                                            gLayerTypeSoil,
                                            gTotalAirPollution->gLayerAirPollution);

        gClock = {0, 0, true, 1, 1, 0};

    }

    //CITY
    std::shared_ptr<gIGrid<uint8_t>> gLayerTypeGen;
    //VAL 0: NOTHING;
    //VAL 1: CIVIL BUILDING;
    //VAL 2: FACTORY BUILDING;
    //VAL 3: HEAVY FACTORY BUILDING;
    //VAL 4: FIELDS;
    //VAL 5: ROADS;

    std::shared_ptr<gIGrid<uint8_t>> gLayerTypeSoil;
    // NOTHING
    // TYPE 1 Mixed
    // TYPE 2 Mixed
    // TYPE 3 Mixed
    // TYPE 1 Industrial
    // TYPE 2 Industrial
    // TYPE 1 Farm
    // TYPE 1 Protected
    // TYPE 1 Obstacle, can be circumvented
    // TYPE 2 Obstacle, solid

    std::shared_ptr<gIGrid<uint32_t>> gLayerCurStruct;
    std::shared_ptr<gIGrid<rNode *>> gLayerNextRoad;

    //AIR POLLUTION
    std::shared_ptr<gMainAirPollution> gTotalAirPollution;

    //ROADS
    std::vector<std::vector<rNode *>> gLayerRoads;
    std::shared_ptr<gIGrid<uint8_t>> gLayerTransit;
    std::vector<rNode *> gTotalNodes;

    std::shared_ptr<rPileMenus> rInteraction;

    rGUIClock::objClockValues gClock;
    uint8_t pTickMinute = 0;

    void tick() {

        pTickMinute++;
        if (pTickMinute > 5) {
            pTickMinute = 0;
            gClock.rVMinute = (gClock.rVMinute + 5) % 60;
            if (gClock.rVMinute == 0) {
                gClock.rVHour = (gClock.rVHour + 1) % 13;
                if (gClock.rVHour == 0) {
                    gClock.rVHour++;
                    if (!gClock.rVIsAM) {
                        gClock.rVDay = (gClock.rVDay + 1) % 31;
                        if (gClock.rVDay == 0)
                            gClock.rVDay++;
                        if (gClock.rVMonth == 0) {
                            gClock.rVMonth = (gClock.rVMonth + 1) % 13;
                            if (gClock.rVMonth == 0)
                                gClock.rVMonth++;
                            gClock.rVYear++;
                        }
                    }
                    gClock.rVIsAM = !gClock.rVIsAM;
                }
            }


            uint32_t tReduced = gClock.rVMinute / 5 + gClock.rVHour * 12 + (gClock.rVIsAM ? 0 : 144);
            sComp->tick(tReduced);
            auto newRoutes = sTCivil->getEndStartPoints(tReduced);
            for (auto r: newRoutes) {
                uint32_t locId = gLayerRoads[r.c_REnd.first][r.c_REnd.second]->refCompressed->locIdNode;
                uint16_t blocId = gLayerRoads[r.c_REnd.first][r.c_REnd.second]->refCompressed->rBlock;
                gLayerRoads[r.c_RStart.first][r.c_RStart.second]->refCompressed->addNewCar(locId, blocId);
            }
            //   PER EL TICK DE sTOTALEMPLOYEE;
            rInteraction->gClock->setClock(gClock);
            gTotalAirPollution->tick(gLayerTypeGen);
        }


        for (const std::shared_ptr<rRNodeI> &node: rListRRoads) {
            node->sendInformationStart();
            node->tick();
            node->sendNewInformation();
        }
    }

    void completedStartGrid() {
        extractRoadsFromLayer();
        for (int i = 0; i < 2000; i++) {
            for (const std::shared_ptr<rRNodeI> &node: rListRRoads) {
                node->sendInformationStart();
                node->sendNewInformation();
            }
        }
        rInfoDist::seeMatrix();
    }

    void completedStartCompanies(const std::vector<std::vector<std::pair<int, int>>> &gPosCompanies) {
        sComp->completedStartCompanies(gPosCompanies);
    }


    //MEMORY:
    std::shared_ptr<sMCompany> sComp;
    std::shared_ptr<sTotalRoutes> sTCivil = std::make_shared<sTotalRoutes>();

private:
    void extractRoadsFromLayer() {
        std::pair<std::vector<rNode *>, std::vector<std::vector<rNode *>>> r = rNodeFromGrid<uint8_t>::givenGrid(
                gLayerTypeGen, 2);
        gLayerRoads = r.second;
        std::cout << r.first.size() << std::endl;
        for (const auto &rNode: r.first) {
            rListRRoads.merge(rTransRNodeToRRNode().conversion(rNode, sqrt(sizeL), sizeL, gLayerTransit));
            rInfoDist::initializeMatrix(sizeL / sqrt(sizeL) * sizeL / sqrt(sizeL), sizeL, rListRRoads.size());
        }

        gBaseToNearestRoad::givenMatRef(gLayerNextRoad, gLayerRoads, gLayerTypeGen);

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
        sTCivil->addRuteCivil({{{64,75}, {8,50}}, 5, 250});
    };
    std::list<std::shared_ptr<rRNodeI>> rListRRoads;
    int sizeL;

    void choseFromVector(const int size, int &first, int &second) {
        first = rand() % size;
        second = rand() % (size - 1);

        if (second >= first) {
            ++second;
        }
    }
};

#endif //CITYOFWEIRDFISHES_SMAINSIMULATOR_H

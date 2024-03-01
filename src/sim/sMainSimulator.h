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

class sMainSimulator {

public:
    explicit sMainSimulator(int lSize) : sizeL(lSize) {
        gLayerTypeBuild = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        gLayerTypeGen = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        gLayerCurStruct = std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(lSize, lSize, 0));
        gLayerAirPollution = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        gLayerTransit = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        gLayerOwnership = std::make_shared<gBasicGrid<std::list<uint32_t>>>
                (gBasicGrid<std::list<uint32_t>>(lSize, lSize, {}));
        gLayerNextRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(lSize, lSize, nullptr));
    }


    //CITY
    std::shared_ptr<gIGrid<uint8_t>> gLayerTypeGen;
    std::shared_ptr<gIGrid<uint8_t>> gLayerTypeBuild;
    std::shared_ptr<gIGrid<std::list<uint32_t>>> gLayerOwnership;
    std::shared_ptr<gIGrid<uint32_t>> gLayerCurStruct;
    std::shared_ptr<gIGrid<rNode *>> gLayerNextRoad;

    //AIR POLLUTION
    std::shared_ptr<gIGrid<uint8_t>> gLayerAirPollution;

    //ROADS
    std::vector<std::vector<rNode *>> gLayerRoads;
    std::shared_ptr<gIGrid<uint8_t>> gLayerTransit;


    void tick() {
        for (const std::shared_ptr<rRNodeI> &node: rListRRoads) {
            node->sendInformationStart();
            node->tick();
            node->sendNewInformation();
        }
    }

    void completedStartGrid() {
        extractRoadsFromLayer();
        for(int i = 0; i < 2000; i++){
            for (const std::shared_ptr<rRNodeI> &node: rListRRoads) {
                node->sendInformationStart();
                node->sendNewInformation();
            }
        }
        rInfoDist::seeMatrix();
    }


    //MEMORY:
    std::shared_ptr<sTotalCompany> sTComp = std::make_shared<sTotalCompany>(100);

private:
    void extractRoadsFromLayer() {
        std::pair<std::vector<rNode *>, std::vector<std::vector<rNode *>>> r = rNodeFromGrid<uint8_t>::givenGrid(
                gLayerTypeGen, 2);
        gLayerRoads = r.second;
        std::cout << r.first.size() << std::endl;
        if (!r.first.empty()) {
            rNode *rOne = r.first[0];
            rListRRoads = rTransRNodeToRRNode().conversion(rOne, sqrt(sizeL), sizeL, gLayerTransit);
            rInfoDist::initializeMatrix(sizeL / sqrt(sizeL) * sizeL / sqrt(sizeL), sizeL, rListRRoads.size());
        }

        sTComp->addCompanyAtPosition(gLayerOwnership, {25, 25});
        gBaseToNearestRoad::givenMatRef(gLayerNextRoad,gLayerRoads, gLayerTypeGen);
    };
    std::list<std::shared_ptr<rRNodeI>> rListRRoads;
    int sizeL;
};

#endif //CITYOFWEIRDFISHES_SMAINSIMULATOR_H

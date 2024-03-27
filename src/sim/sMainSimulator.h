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
#include "behaviour/metro/gMainUnderground.h"
#include "behaviour/gTerrainGrid.h"
#include "roads/sMainRoads.h"
#include "behaviour/market/sMainEvaluator.h"
#include "behaviour/clock/sMainClock.h"

class sMainSimulator {

public:
    explicit sMainSimulator(int lSize) {
        gMainTerrain = std::make_shared<gTerrainGrid>(lSize);
        gLayerCurStruct = std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(lSize, lSize, 0));
        gLayerNextRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(lSize, lSize, nullptr));
        gTotalAirPollution = std::make_shared<gMainAirPollution>(lSize);
        gTotalUnderground = std::make_shared<gMainUnderground>(lSize);

        rMEvaluator = std::make_shared<sMainEvaluator>();
        gMainRoads = std::make_shared<sMainRoads>(lSize, gMainTerrain);
        sMCivil = std::make_shared<sMainCivil>(gMainRoads);
        sMEvaluator = std::make_shared<sMainEvaluator>();
        sComp = std::make_shared<sMCompany>(lSize, sMCivil,
                                            gMainTerrain,
                                            gTotalAirPollution->gLayerAirPollution,
                                            sMEvaluator);
        sMClock = std::make_shared<sMainClock>();
    }

    //TERRAIN BASIC;
    std::shared_ptr<gTerrainGrid> gMainTerrain;

    std::shared_ptr<gIGrid<uint32_t>> gLayerCurStruct;
    std::shared_ptr<gIGrid<rNode *>> gLayerNextRoad;

    std::shared_ptr<sMainEvaluator> sMEvaluator;


    //AIR POLLUTION
    std::shared_ptr<gMainAirPollution> gTotalAirPollution;

    //UNDERGROUND
    std::shared_ptr<gMainUnderground> gTotalUnderground;

    //ROADS
    std::shared_ptr<sMainRoads>  gMainRoads;

    std::shared_ptr<rPileMenus> rInteraction;

    std::shared_ptr<sMainEvaluator> rMEvaluator;

    std::shared_ptr<sMainClock> sMClock;


    //MEMORY:
    std::shared_ptr<sMainCivil> sMCivil;
    std::shared_ptr<sMCompany> sComp;

    std::shared_ptr<sTotalRoutes> sTCivil = std::make_shared<sTotalRoutes>();

    void tick() {
        if (sMClock->isReducedTick()) {
            allTicksReduced(sMClock->getReduced(), sMClock->getDate());
            rInteraction->gClock->setClock(sMClock->getClock());
        }
        sMClock->tick();

        gMainRoads->tick();
    }

    void completedSetupStage() {
        gMainRoads->completedStartGrid(gMainTerrain);
        gMainTerrain->setupLists();
        //sComp->addNewCompany(sMCompany::sMComp_TypeCompany::SMComp_Factory);
    }

    void completedStartCompanies(const std::vector<retObjCompany>& gCompPositions) {
        sComp->completedStartCompanies(gCompPositions);
    }

private:

    void allTicksReduced(const uint32_t tReduced, const uint32_t tDate) {
        gMainRoads->tickReduced(tReduced, tDate);
        sComp->tickReduced(tReduced, tDate);
        gTotalAirPollution->tickReduced(gMainTerrain->gTG_TypeGen);
        gTotalUnderground->tickReduced(tReduced);
        sMEvaluator->tickReduced(tReduced, tDate);
    }
};

#endif //CITYOFWEIRDFISHES_SMAINSIMULATOR_H

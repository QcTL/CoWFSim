//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SSIMULATORMAIN_H
#define CITYOFWEIRDFISHES_SSIMULATORMAIN_H

#include <cstdint>
#include <memory>
#include <vector>
#include "roads/node/rNode.h"
#include "roads/rTransRNodeToRRNode.h"
#include "roads/rNodeFromGrid.h"
#include "structure/grids/gBasicGrid.h"
#include "structure/grids/gIGrid.h"
#include "structure/obj/sCommon.h"
#include "behaviour/company/sCompanyTotal.h"
#include "structure/grids/transformation/gBaseToNearestRoad.h"
#include "../display/rPileMenus.h"
#include "structure/obj/sRoutesStorage.h"
#include "behaviour/company/sCompanyMain.h"
#include "behaviour/airPollution/sgAirPollutionMain.h"
#include "behaviour/metro/sgUndergroundMain.h"
#include "behaviour/sgTerrain.h"
#include "roads/sgRoadsMain.h"
#include "behaviour/market/sEvaluatorMain.h"
#include "behaviour/clock/sClockMain.h"

class sSimulatorMain {

public:
    explicit sSimulatorMain(int lSize) {
        gMainTerrain = std::make_shared<sgTerrain>(lSize);
        gLayerCurStruct = std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(lSize, lSize, 0));
        gLayerNextRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(lSize, lSize, nullptr));
        gTotalAirPollution = std::make_shared<sgAirPollutionMain>(lSize);
        gTotalUnderground = std::make_shared<sgUndergroundMain>(lSize);

        rMEvaluator = std::make_shared<sEvaluatorMain>();
        gMainRoads = std::make_shared<sgRoadsMain>(lSize, gMainTerrain);
        sMCivil = std::make_shared<sCivilMain>(gMainRoads, gTotalUnderground);
        sMEvaluator = std::make_shared<sEvaluatorMain>();
        sComp = std::make_shared<sCompanyMain>(lSize, sMCivil,
                                               gMainTerrain,
                                               gTotalAirPollution->gLayerAirPollution,
                                               sMEvaluator);
        sMClock = std::make_shared<sClockMain>();
    }

    //TERRAIN BASIC;
    std::shared_ptr<sgTerrain> gMainTerrain;

    std::shared_ptr<gIGrid<uint32_t>> gLayerCurStruct;
    std::shared_ptr<gIGrid<rNode *>> gLayerNextRoad;

    std::shared_ptr<sEvaluatorMain> sMEvaluator;


    //AIR POLLUTION
    std::shared_ptr<sgAirPollutionMain> gTotalAirPollution;

    //UNDERGROUND
    std::shared_ptr<sgUndergroundMain> gTotalUnderground;

    //ROADS
    std::shared_ptr<sgRoadsMain> gMainRoads;

    std::shared_ptr<rPileMenus> rInteraction;

    std::shared_ptr<sEvaluatorMain> rMEvaluator;

    std::shared_ptr<sClockMain> sMClock;


    //MEMORY:
    std::shared_ptr<sCivilMain> sMCivil;
    std::shared_ptr<sCompanyMain> sComp;

    std::shared_ptr<sRoutesStorage> sTCivil = std::make_shared<sRoutesStorage>();

    void tick() {
        if (sMClock->isReducedTick()) {
            allTicksReduced(sMClock->getReduced(), sMClock->getDate());
            rInteraction->gClock->setClock(sMClock->getClock());
        }
        sMClock->tick();
        gTotalUnderground->tick();
        gMainRoads->tick();
    }

    void completedSetupStage() {
        gMainRoads->completedStartGrid(gMainTerrain);
        gMainTerrain->setupLists();
        //sComp->addNewCompany(sCompanyMain::sCM_strStyleCompany::SCM_strFactory);
    }

    void completedStartCompanies(const std::vector<retObjCompany> &gCompPositions) {
        sComp->completedStartCompanies(gCompPositions);
    }

private:

    void allTicksReduced(const uint32_t inRTime, const uint32_t inTDate) {
        gMainRoads->tickReduced(inRTime, inTDate);
        sComp->tickReduced(inRTime, inTDate);
        gTotalAirPollution->tickReduced(gMainTerrain->gTG_TypeGen);
        sMEvaluator->tickReduced(inRTime, inTDate);
    }
};

#endif //CITYOFWEIRDFISHES_SSIMULATORMAIN_H

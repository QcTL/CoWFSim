//
// Created by Laminar on 11/02/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GVEH_H
#define CITYOFWEIRDFISHES_TST_GVEH_H

#include <memory>
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBaseToPattern.h"
#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/roads/rNodeFromGrid.h"
#include "../display/menus/implementation/rBaseMenu.h"
#include "../sim/layers/implementations/sLayerType.h"
#include "../common/sContainerMain.h"


int tst_gVeh() {
    std::shared_ptr<sSimulatorMain> sMS = std::make_shared<sSimulatorMain>(100);

    std::shared_ptr<gIGrid<uint8_t>> gB = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(100, 100, 0));
    std::shared_ptr<gIGrid<uint8_t>> gTransit = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(100, 100, 0));


    gB->set(0, 2, 6);
    gB->set(1, 0, 6);
    gB->set(1, 1, 6);
    gB->set(1, 2, 6);
    gB->set(1, 3, 6);
    gB->set(2, 2, 6);


    gBaseToPattern<uint8_t> gBP(gB,
                                gBaseToPattern<uint8_t>::gPatternType::gBPBlobSquares,
                                gBaseToPattern<uint8_t>::gPatternParameters(4, 4, 25, 25),
                                6);

    sMS->sSM_groupLand->gL_gTerrain->gTG_TypeGen = gB;
    sMS->sSM_groupLand->gL_gAirPollution->gLayerAirPollution = gB;
    sMS->completedSetupStage();

/*
    sMS->sSM_gMainRoads->gLayerRoads[0][0]->refCompressed->addNewCar(
            sMS->sSM_gMainRoads->gLayerRoads[75][75]->refCompressed->locIdNode,
            sMS->sSM_gMainRoads->gLayerRoads[75][75]->refCompressed->rBlock);
*/

    //START TRANSIT;
    sContainerMain sMC(sMS);

    sMC.gameLoop();
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GVEH_H

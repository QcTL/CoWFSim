//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GEXTRACTROADS_H
#define CITYOFWEIRDFISHES_TST_GEXTRACTROADS_H

#include <memory>
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBaseToPattern.h"
#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/roads/rNodeFromGrid.h"
#include "../display/menus/implementation/rBaseMenu.h"

int tst_gExtractRoads() {

    std::shared_ptr<sSimulatorMain> sMS = std::make_shared<sSimulatorMain>(50);
    std::shared_ptr<gIGrid<uint8_t>> gB = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(50, 50, 0));
    gBaseToPattern<uint8_t> gBP(gB,
                       gBaseToPattern<uint8_t>::gPatternType::gBPBlobSquares,
                       gBaseToPattern<uint8_t>::gPatternParameters(3, 3, 1, 1));

    gB->set(30,0, 1);
    gB->set(31,0, 1);
    gB->set(32,0, 1);

    rNodeFromGrid<uint8_t>::givenGrid(gB, 1);

    sMS->gLayerAirPollution = gB;
    sMS->completedSetupStage();


    std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(sMS->gLayerAirPollution,
                                                                       sMS->gMainTerrain->gLayerCurStruct, sMS->gLayerTransit);
    //MENUS
    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
    std::shared_ptr<rBaseMenu> rBasic = std::make_shared<rBaseMenu>(rBaseMenu(pPM, sMS->gLayerTypeGen,
                                                                              sMS->gLayerRoads,
                                                                              sMS->sSM_sCompany->gLayerOwnership, sMS->sSM_sCompany->sTComp));
    pPM->addMenuTop(rBasic);
    rGlobal rG(gSimL, pPM);
    rG.setUp();
    while (rG.isOpen) {
        rG.loop();
        sMS->tick();
    }
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GEXTRACTROADS_H

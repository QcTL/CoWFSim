//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GEXTRACTROADSCOMPRESSED_H
#define CITYOFWEIRDFISHES_TST_GEXTRACTROADSCOMPRESSED_H

#include <memory>
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBaseToPattern.h"
#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/roads/rNodeFromGrid.h"
#include "../display/menus/implementation/rBaseMenu.h"

int tst_gExtractRoadsCompressed() {

    std::shared_ptr<sMainSimulator> sMS = std::make_shared<sMainSimulator>(50);
    std::shared_ptr<gIGrid<uint8_t>> gB = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(50, 50, 0));

    gB->set(0,0, 1);
    gB->set(1,0, 1);
    gB->set(2,0, 1);
    gB->set(3,0, 1);
    gB->set(4,0, 1);

    gB->set(2,1, 1);
    gB->set(4,1, 1);

    gB->set(2,2, 1);
    gB->set(3,2, 1);
    gB->set(4,2, 1);
    gB->set(5,2, 1);
    gB->set(6,2, 1);
    gB->set(7,2, 1);

    gB->set(6,3, 1);

    sMS->gLayerAirPollution = gB;
    sMS->completedStartGrid();


    std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(sMS->gLayerAirPollution,
                                                                       sMS->gLayerCurStruct, sMS->gLayerTransit);
    //MENUS
    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
    std::shared_ptr<rBaseMenu> rBasic = std::make_shared<rBaseMenu>(rBaseMenu(pPM, sMS->gLayerTypeGen,
                                                                              sMS->gLayerRoads,
                                                                              sMS->gLayerOwnership, sMS->sTComp));
    pPM->addMenuTop(rBasic);
    rGlobal rG(gSimL, pPM);
    rG.setUp();
    while (rG.isOpen) {
        rG.loop();
        sMS->tick();
    }
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GEXTRACTROADSCOMPRESSED_H

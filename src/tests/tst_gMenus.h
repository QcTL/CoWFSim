//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GMENUS_H
#define CITYOFWEIRDFISHES_TST_GMENUS_H


#include "../display/rGlobal.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/sSimulatorMain.h"
#include "../display/menus/implementation/rBaseMenu.h"

int tst_gMenus() {

    std::shared_ptr<gIGrid<uint8_t>> gB = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(30, 30, 255));
    std::shared_ptr<sSimulatorMain> sMS = std::make_shared<sSimulatorMain>(30);


    std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(sMS->gTotalAirPollution->gLayerAirPollution ,
                                                                       sMS->gLayerCurStruct, sMS->gLayerTransit);
    //MENUS
    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
    std::shared_ptr<rBaseMenu> rBasic = std::make_shared<rBaseMenu>(rBaseMenu(pPM, sMS->gLayerTypeGen,
                                                                              sMS->gLayerRoads,
                                                                              sMS->sComp->gLayerOwnership, sMS->sComp->sTComp));
    pPM->addMenuTop(rBasic);
    rGlobal rG(gSimL, pPM);
    rG.setUp();
    while (rG.isOpen) {
        rG.loop();
        sMS->tick();
    }
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GMENUS_H

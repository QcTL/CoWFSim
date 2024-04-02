//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICRIVER_H
#define CITYOFWEIRDFISHES_TST_GBASICRIVER_H

#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/transformation/gBaseToRiver.h"
#include "../sim/sSimulatorMain.h"
#include "../display/menus/implementation/rBaseMenu.h"

int tst_gBasicRiver() {
    std::shared_ptr<sSimulatorMain> sMS = std::make_shared<sSimulatorMain>(105);
    std::shared_ptr<gIGrid<uint8_t>> gB =std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(105, 105, 3));
    gBaseToRiver<uint8_t> gBTR(gB,20,1.6, 100);

    sMS->gLayerAirPollution = gB;

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
#endif //CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H

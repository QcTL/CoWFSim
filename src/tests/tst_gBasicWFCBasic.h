//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICWFCBASIC_H
#define CITYOFWEIRDFISHES_TST_GBASICWFCBASIC_H

#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"
#include "../sim/structure/grids/transformation/gBaseToWFC.h"
#include "../sim/sSimulatorMain.h"
#include "../display/menus/implementation/rBaseMenu.h"

int tst_gBasicWFCBasic() {


    int rRows = 100;
    int rCols = 100;
    std::shared_ptr<sSimulatorMain> sMS = std::make_shared<sSimulatorMain>(rRows);
    //MASK:
    std::shared_ptr<gIGrid<bool>> b = std::make_shared<gBasicGrid<bool>>(gBasicGrid<bool>(rRows, rCols, -1));

    std::shared_ptr<gIGrid<uint8_t>> gReal =std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(rRows, rCols, 0));

    gBaseToWFC wfc(gReal, {{1,3},{2,7},{3,6}},{rRows/2,rCols/2}, 1);

    sMS->gLayerAirPollution = gReal;

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
#endif //CITYOFWEIRDFISHES_TST_GBASICWFCBASIC_H

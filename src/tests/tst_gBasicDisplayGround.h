//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICDISPLAYGROUND_H
#define CITYOFWEIRDFISHES_TST_GBASICDISPLAYGROUND_H

#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"

int tst_gBasicDisplayGround() {


    std::shared_ptr<gIGrid<uint8_t>> gB =std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(30, 30, 255));
    std::shared_ptr<sMainSimulator> sMS = std::make_shared<sMainSimulator>(30);

    std::vector<gtmElement> vElem = {
            gtmElement(0.0, 0, 0.0, 0),
            gtmElement(0.5, 0, 0.05, 0),
            gtmElement(0.15, 0, 0.3, 0),};

    std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(sMS->gLayerAirPollution,
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
#endif //CITYOFWEIRDFISHES_TST_GBASICDISPLAYGROUND_H

//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICPATTERN_H
#define CITYOFWEIRDFISHES_TST_GBASICPATTERN_H

#include <memory>
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBaseToPattern.h"
#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/sMainSimulator.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../display/menus/implementation/rBaseMenu.h"

int tst_gBasicPattern() {
    std::shared_ptr<sMainSimulator> sMS = std::make_shared<sMainSimulator>(50);
    std::shared_ptr<gIGrid<uint8_t>> gB = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(50, 50, 0));
    gBaseToPattern gBP(gB,
                       gBaseToPattern<uint8_t>::gPatternType::gBPBlobSquares,
                       gBaseToPattern<uint8_t>::gPatternParameters(3, 3, 3, 3));

    sMS->gLayerAirPollution = gB;

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

#endif //CITYOFWEIRDFISHES_TST_GBASICPATTERN_H

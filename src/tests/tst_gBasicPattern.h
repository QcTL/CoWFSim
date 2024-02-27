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

int tst_gBasicPattern() {
    std::shared_ptr<gIGrid<int>> gB = std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(50, 50, 0));
    gBaseToPattern gBP(gB,
                       gBaseToPattern<int>::gPatternType::gBPBlobSquares,
                       gBaseToPattern<int>::gPatternParameters(3, 3, 3, 3));

    std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gB));
    gLAP->setTransformation({0, 1, 2, 3, 4, 5});
    std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(gLAP, nullptr, gB->rangeUse());
    gSimL->switchActual(gSimLayersTypes::G_AIRPOLLUTION);

    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
    std::shared_ptr<rBaseMenu> rBasic = std::make_shared<rBaseMenu>(
            pPM, rIMenu::rRelativePos::pBottomRight);
    pPM->addMenuTop(rBasic);


    rGlobal rG(gSimL, pPM);
    rG.setUp();
    while (rG.isOpen) {
        rG.loop();
    }

    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GBASICPATTERN_H

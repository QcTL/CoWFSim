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

int tst_gBasicRiver() {


    std::shared_ptr<gIGrid<int>> gB =std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(105, 105, 3));

    gBaseToRiver<int> gBTR(gB,20,1.6, 100);

    std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gB));
    gLAP->setTransformation({0,1,2,3,4,5});
    std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(gSimLayers(gLAP));

    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>();
    rGlobal rG(gSimL, pPM);
    rG.setUp();
    while(rG.isOpen) {
        rG.loop();
    }
    return 0;
}
#endif //CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H

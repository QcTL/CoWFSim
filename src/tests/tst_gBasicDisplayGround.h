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



    /*
    std::vector<gtmElement> vElem = {
            gtmElement(0.0, 0, 0.0, 0),
            gtmElement(0.5, 0, 0.05, 0),
            gtmElement(0.15, 0, 0.3, 0),};

    gBaseToGradientMinimum gBGM(vElem, {15, 15}, gB); // Pass ptr by reference
    gBGM.generateV2();

    std::cout << *dynamic_cast<gBasicGrid<int> *>(gB.get()) << std::endl;
    std::cout << "------------" << std::endl;
*/

    std::shared_ptr<gIGrid<int>> gAP =std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(30, 30, 0));
    std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gAP));
    gLAP->setTransformation({0,1,2,3,4,5});

    std::shared_ptr<gIGrid<uint32_t>> gC =std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(30, 30, ((uint32_t)(uint8_t)strtol("00000000", NULL, 2)) << 24));
    std::shared_ptr<gLayerCity> gLC = std::make_shared<gLayerCity>(gLayerCity(gC));
    std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(gLAP, gLC, gAP->rangeUse());

    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
    rGlobal rG(gSimL, pPM);
    rG.setUp();
    while(rG.isOpen) {
        rG.loop();
    }
    return 0;
}
#endif //CITYOFWEIRDFISHES_TST_GBASICDISPLAYGROUND_H

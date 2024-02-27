//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H
#define CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H

#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"

int tst_gBasicDisplay() {


    std::shared_ptr<gIGrid<int>> gB =std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(30, 30, -1));

    std::vector<gtmElement> vElem = {
            gtmElement(0.0, 0, 0.0, 0),
            gtmElement(0.5, 0, 0.05, 0),
            gtmElement(0.15, 0, 0.3, 0),};

    gBaseToGradientMinimum gBGM(vElem, {15, 15}, gB); // Pass ptr by reference
    gBGM.generateV2();

    std::cout << *dynamic_cast<gBasicGrid<int> *>(gB.get()) << std::endl;
    std::cout << "------------" << std::endl;



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

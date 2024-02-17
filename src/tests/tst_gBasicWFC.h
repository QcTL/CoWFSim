//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H
#define CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H

#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../display/layers/gSimLayers.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"
#include "../sim/structure/grids/transformation/gBaseToWFC.h"

int tst_gBasicWFC() {


    std::shared_ptr<gIGrid<int>> gB =std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(30, 30, -1));

    std::vector<gtmElement> vElem = {
            gtmElement(0.0, 0, 0.0, 0),
            gtmElement(0.1, 0, 0.05, 0),
            gtmElement(0.15, 0, 0.3, 0),};

    gBaseToGradientMinimum gBGM(vElem, {15, 15}, gB, 15); // Pass ptr by reference
    gBGM.generateV2();

    std::cout << *dynamic_cast<gBasicGrid<int> *>(gB.get()) << std::endl;
    std::cout << "------------" << std::endl;


    //MASK:
    std::shared_ptr<gIGrid<bool>> b = BasicTransformations::genMaskFromGrid(gB, {1,2});

    std::cout << *dynamic_cast<gBasicGrid<bool> *>(b.get()) << std::endl;
    std::cout << "------------" << std::endl;

    std::shared_ptr<gIGrid<int>> gReal =std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(30, 30, 0));
    gBaseToWFC wfc(gReal, {{1,3},{2,7},{3,6}},{15,15}, 1, b);

    std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gReal));
    gLAP->setTransformation({0,1,2,3,4,5});
    std::shared_ptr<gSimLayers> gSimL = std::make_shared<gSimLayers>(gSimLayers(gLAP));

    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>();
    rGlobal rG(gSimL, pPM);
    rG.setUp();
    while(rG.isOpen) {
        rG.loop();
    }
    return 0;
}
#endif //CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H

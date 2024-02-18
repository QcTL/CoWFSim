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

int tst_gBasicWFCBasic() {


    int rRows = 100;
    int rCols = 100;

    //MASK:
    std::shared_ptr<gIGrid<bool>> b = std::make_shared<gBasicGrid<bool>>(gBasicGrid<bool>(rRows, rCols, -1));

    std::shared_ptr<gIGrid<int>> gReal =std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(rRows, rCols, 0));

    gBaseToWFC wfc(gReal, {{1,3},{2,7},{3,6}},{rRows/2,rCols/2}, 1);

    /*
    std::cout << *dynamic_cast<gBasicGrid<int> *>(gReal.get()) << std::endl;
    std::cout << "------------" << std::endl;
    */

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

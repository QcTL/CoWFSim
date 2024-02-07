//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H
#define CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H


#include "../display/menus/implementations/rSelOptMenu.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/gBasicGrid.h"

int tst_gMenus() {


    std::shared_ptr<gIGrid<int>> gB =std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(10, 10, 0));
    std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gB));
    gLAP->setTransformation({0,1,2,3,4,5});
    std::shared_ptr<gSimLayers> gSimL = std::make_shared<gSimLayers>(gSimLayers(gLAP));

    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>();
    std::shared_ptr<rSelOptMenu> rSom = std::make_shared<rSelOptMenu>("d_mSelectLayer", rIMenu::rRelativePos::pBottomLeft);
    pPM->addMenuTop(rSom);



    rGlobal rG(gSimL, pPM);
    rG.setUp();
    int i = 0;
    while(rG.isOpen) {
        i ++;
        rSom->setNewSel((i/ 1000) % 5);
        rG.loop();
    }
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H

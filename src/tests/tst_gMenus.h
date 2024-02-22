//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GMENUS_H
#define CITYOFWEIRDFISHES_TST_GMENUS_H


#include "../display/menus/implementations/rSelOptMenu.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../display/menus/implementations/rBaseMenu.h"

int tst_gMenus() {


    //AIR
    std::shared_ptr<gIGrid<int>> gB =std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(10, 10, 3));
    std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gB));
    gLAP->setTransformation({0,1,2,3,4,5});

    //CITY
    std::shared_ptr<gIGrid<uint32_t>> gCity =std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(10, 10, 0));
    std::shared_ptr<gLayerCity> gLC = std::make_shared<gLayerCity>(gLayerCity(gCity));

    //DISPLAY
    std::shared_ptr<gSimLayers> gSimL = std::make_shared<gSimLayers>(gLAP, gLC, gB->rangeUse());

    //MENUS
    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
    std::shared_ptr<rBaseMenu> rBasic =  std::make_shared<rBaseMenu>(
            pPM, rIMenu::rRelativePos::pBottomRight);
    pPM->addMenuTop(rBasic);


    rGlobal rG(gSimL, pPM);
    rG.setUp();
    int i = 0;
    while(rG.isOpen) {
        i ++;
        //rSom->setNewSel((i/ 1000) % 5);
        rG.loop();
    }
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GMENUS_H

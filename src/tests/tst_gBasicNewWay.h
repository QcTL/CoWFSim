//
// Created by Laminar on 29/02/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICNEWWAY_H
#define CITYOFWEIRDFISHES_TST_GBASICNEWWAY_H

#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/sMainSimulator.h"
#include "../sim/structure/grids/transformation/sGridToSimulator.h"
#include "../display/menus/implementation/rBaseMenu.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"
#include "../sim/layers/implementations/sLayerType.h"

int tst_gBasicNewWay( const std::map<std::string, std::string> &mValues) {

    uint32_t lSizeGrid = 0;
    if (mValues.at("Mida_Simulacio") == "Petita")
        lSizeGrid = 100;
    else if (mValues.at("Mida_Simulacio") == "Mitjana")
        lSizeGrid = 150;
    else if (mValues.at("Mida_Simulacio") == "Gran")
        lSizeGrid = 200;
    else if (mValues.at("Mida_Simulacio") == "Molt_Gran")
        lSizeGrid = 250;

    std::shared_ptr<sMainSimulator> sMS = std::make_shared<sMainSimulator>(lSizeGrid);

    sMS->gLayerAirPollution = sLayerType::gen(lSizeGrid, mValues);

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

#endif //CITYOFWEIRDFISHES_TST_GBASICNEWWAY_H

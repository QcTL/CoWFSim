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
#include "../sim/sSimulatorMain.h"
#include "../sim/structure/grids/transformation/sGridToSimulator.h"
#include "../display/menus/implementation/rBaseMenu.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"
#include "../sim/layers/implementations/sLayerType.h"

int tst_gBasicNewWay(const std::map<std::string, std::string> &mValues) {

    uint32_t lSizeGrid = 0;
    if (mValues.at("Mida_Simulacio") == "Petita")
        lSizeGrid = 100;
    else if (mValues.at("Mida_Simulacio") == "Mitjana")
        lSizeGrid = 150;
    else if (mValues.at("Mida_Simulacio") == "Gran")
        lSizeGrid = 200;
    else if (mValues.at("Mida_Simulacio") == "Molt_Gran")
        lSizeGrid = 250;

    std::shared_ptr<sSimulatorMain> sMS = std::make_shared<sSimulatorMain>(lSizeGrid);

    sMS->gTotalAirPollution->gLayerAirPollution = sLayerType::gen(lSizeGrid, mValues);

    sMainContainer sMC(sMS);

    sMC.gameLoop();
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GBASICNEWWAY_H

//
// Created by Laminar on 06/03/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GWINDEXPANSION_H
#define CITYOFWEIRDFISHES_TST_GWINDEXPANSION_H

#include "../sim/structure/grids/transformation/gBaseToRiver.h"
#include "../sim/structure/grids/transformation/gBaseToLineRoads.h"
#include "../sim/structure/grids/transformation/gBaseToPattern.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"
#include "../sim/sMainSimulator.h"
#include "../sim/structure/grids/transformation/sGridToSimulator.h"
#include "../display/rGlobal.h"
#include "../display/menus/implementation/rBaseMenu.h"
#include "../sim/structure/grids/transformation/gBaseToField.h"
#include "../sim/layers/implementations/sLayerType.h"
#include "../sim/layers/implementations/sLayerCells.h"
#include "../common/sMainContainer.h"

int tst_gWindExpansion() {
    uint32_t lSizeGrid = 30;

    std::shared_ptr<sMainSimulator> sMS = std::make_shared<sMainSimulator>(lSizeGrid);

    //sMS->gLayerTypeSoil = sLayerType::gen(lSizeGrid, sMS->gLayerTypeGen, mValues);
    //sMS->gLayerAirPollution = sLayerType::gen(lSizeGrid, sMS->gLayerTypeGen, mValues);

    //sLayerCells::retObjSLayerCells retCells =
    //        sLayerCells::gen(lSizeGrid, sMS->gLayerTypeSoil, sMS->gLayerTypeGen, mValues);
    //sMS->gLayerCurStruct = retCells.gMatrix;

    sMS->gLayerTypeGen->set(3,3,3);
    sMS->gLayerTypeGen->set(15,15,3);
    sMS->gLayerTypeGen->set(16,15,3);
    sMS->completedStartGrid();
    //sMS->completedStartCompanies(retCells.gCompanyPositions);

    sMainContainer sMC(sMS);

    sMC.gameLoop();
    return 0;
};
#endif //CITYOFWEIRDFISHES_TST_GWINDEXPANSION_H

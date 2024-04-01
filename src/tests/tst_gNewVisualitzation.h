//
// Created by Laminar on 12/03/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GNEWVISUALITZATION_H
#define CITYOFWEIRDFISHES_TST_GNEWVISUALITZATION_H

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
#include "../common/sContainerMain.h"
#include "../sim/layers/implementations/sLayerCells.h"

int tst_gNewVisualization(const std::map<std::string, std::string> &mValues) {
    int lSizeGrid = 10;

    std::shared_ptr<sSimulatorMain> sMS = std::make_shared<sSimulatorMain>(lSizeGrid);

    //sMS->gLayerTypeSoil = sLayerType::gBTGM_gen(lSizeGrid, sMS->gLayerTypeGen, mValues);

    sMS->gLayerTypeGen->set(0, 0, 5);
    sMS->gLayerTypeGen->set(1, 0, 5);
    sMS->gLayerTypeGen->set(2, 0, 5);
    sMS->gLayerTypeGen->set(3, 0, 5);
    sMS->gLayerTypeGen->set(0, 1, 5);
    sMS->gLayerTypeGen->set(0, 2, 5);
    sMS->gLayerTypeGen->set(1, 2, 5);
    sMS->gLayerTypeGen->set(2, 1, 6);
    sMS->gLayerTypeGen->set(3, 2, 6);
    sMS->gLayerTypeGen->set(2, 2, 6);

    sMS->gLayerTypeGen->set(3, 1, 5);
    sMS->gLayerTypeGen->set(4, 0, 5);
    sMS->gLayerTypeGen->set(4, 1, 5);
    sMS->gLayerTypeGen->set(4, 2, 6);
    //sMS->gLayerTypeGen->set(20, 20, 6);

    sLayerCells::retObjSLayerCells retCells =
            sLayerCells::gen(lSizeGrid, sMS->gLayerTypeSoil, sMS->gLayerTypeGen, mValues);
    sMS->gMainTerrain->gLayerCurStruct = retCells.gMatrix;


    sMS->completedSetupStage();
    sMS->completedStartCompanies(retCells.gCompanyPositions);

    sContainerMain sMC(sMS);

    sMC.gameLoop();
    return 0;
};

#endif //CITYOFWEIRDFISHES_TST_GNEWVISUALITZATION_H

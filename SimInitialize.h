//
// Created by Laminar on 17/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SIMINITIALIZE_H
#define CITYOFWEIRDFISHES_SIMINITIALIZE_H

#include "src/sim/structure/grids/transformation/gBaseToRiver.h"
#include "src/sim/structure/grids/transformation/gBaseToLineRoads.h"
#include "src/sim/structure/grids/transformation/gBaseToPattern.h"
#include "src/sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "src/sim/structure/grids/transformation/gBasicTransformations.h"
#include "src/sim/sMainSimulator.h"
#include "src/sim/structure/grids/transformation/sGridToSimulator.h"
#include "src/display/rGlobal.h"
#include "src/display/menus/implementation/rBaseMenu.h"
#include "src/sim/structure/grids/transformation/gBaseToField.h"
#include "src/sim/layers/implementations/sLayerType.h"
#include "src/sim/layers/implementations/sLayerCells.h"
#include "src/common/sMainContainer.h"

class SimInitialize {
public:
    static int givenMap(const std::map<std::string, std::string> &mValues) {
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

        sLayerType::returnLayerType tReturn = sLayerType::gen(lSizeGrid, sMS->gLayerTypeGen, mValues);
        sMS->gLayerTypeSoil = tReturn.genTypeSoil;
        //sMS->gTotalAirPollution->gLayerAirPollution = sLayerType::gen(lSizeGrid, sMS->gLayerTypeGen, mValues);

        sLayerCells::retObjSLayerCells retCells =
                sLayerCells::gen(lSizeGrid, sMS->gLayerTypeSoil, sMS->gLayerTypeGen, tReturn.centerClusters, mValues);
        sMS->gLayerCurStruct = retCells.gMatrix;
        sMS->gTotalUnderground->gLayerUnderground = retCells.gUnderground;

        sMS->completedStartGrid();
        sMS->completedStartCompanies({});

        sMainContainer sMC(sMS);

        sMC.gameLoop();
        return 0;
    }


};

#endif //CITYOFWEIRDFISHES_SIMINITIALIZE_H

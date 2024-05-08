//
// Created by Laminar on 17/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SIMINITIALIZE_H
#define CITYOFWEIRDFISHES_SIMINITIALIZE_H

#include <thread>
#include "src/sim/structure/grids/transformation/gBaseToRiver.h"
#include "src/sim/structure/grids/transformation/gBaseToLineRoads.h"
#include "src/sim/structure/grids/transformation/gBaseToPattern.h"
#include "src/sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "src/sim/structure/grids/transformation/gBasicTransformations.h"
#include "src/sim/sSimulatorMain.h"
#include "src/sim/structure/grids/transformation/sGridToSimulator.h"
#include "src/display/rGlobal.h"
#include "src/display/menus/implementation/rBaseMenu.h"
#include "src/sim/structure/grids/transformation/gBaseToField.h"
#include "src/sim/layers/implementations/sLayerType.h"
#include "src/sim/layers/implementations/sLayerCells.h"
#include "src/common/sContainerMain.h"
#include "src/sim/snCommonAtr.h"
#include "src/sim/eyeCatcher/eyeCatcherConnection.h"
#include "src/sim/eyeCatcher/eyeCatcherActive.h"

/**
 * @class SimInitialize
 * @brief This class is responsible to load the starting parameters of the simulation, and execute the main simulator
 */
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

        std::shared_ptr<sSimulatorMain> sMS = std::make_shared<sSimulatorMain>(lSizeGrid);

        sLayerType::returnLayerType tReturn = sLayerType::gen(lSizeGrid,
                                                              mValues, snCommonAtr::getFlagAtr("snCA_Seed"));
        sMS->sSM_groupLand->gL_gTerrain->gTG_TypeSoil = tReturn.genTypeSoil;

        retObjSLayerCells retCells =
                sLayerCells::gen(lSizeGrid, sMS->sSM_groupLand->gL_gTerrain, tReturn.centerClusters, mValues,
                                 snCommonAtr::getFlagAtr("snCA_Seed"));

        sMS->sSM_groupLand->gL_gUnderground->gLayerUnderground = retCells.gUnderground;
        sMS->sSM_groupLand->gL_gUnderground->setPointsTransit(retCells.routesMetro);

        sMS->completedSetupStage();

        sContainerMain sMC(sMS);
        std::shared_ptr<eyeCatcherConnection> _eCC = std::make_shared<eyeCatcherConnection>();
        std::shared_ptr<eyeCatcherActive> _eCA = eyeCatcherActive::getInstance(_eCC);

        sMC.gameLoop();

        return 0;
    }


};

#endif //CITYOFWEIRDFISHES_SIMINITIALIZE_H

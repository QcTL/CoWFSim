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

        sMS->gLayerTypeSoil = sLayerType::gen(lSizeGrid, sMS->gLayerTypeGen, mValues);
        sMS->gLayerAirPollution = sLayerType::gen(lSizeGrid, sMS->gLayerTypeGen, mValues);

        sMS->gLayerCurStruct = sLayerCells::gen(lSizeGrid, sMS->gLayerTypeSoil, sMS->gLayerTypeGen, mValues);

        sMS->completedStartGrid();

        std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(sMS->gLayerAirPollution,
                                                                           sMS->gLayerCurStruct, sMS->gLayerTransit);
        //MENUS
        std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
        std::shared_ptr<rBaseMenu> rBasic = std::make_shared<rBaseMenu>(rBaseMenu(pPM, sMS->gLayerTypeGen,
                                                                                  sMS->gLayerRoads,
                                                                                  sMS->gLayerOwnership, sMS->sTComp));
        pPM->addMenuTop(rBasic);

        //START TRANSIT;
       // uint32_t p1 = sMS->gLayerRoads[22][51]->refCompressed->locIdNode;
       // uint32_t b1 = sMS->gLayerRoads[22][51]->refCompressed->rBlock;
       // sMS->gLayerRoads[43][39]->refCompressed->addNewCar(p1, b1);


        rGlobal rG(gSimL, pPM);
        rG.setUp();
        int p = 0;
        while (rG.isOpen) {
            if (p > 2000) {
                sMS->tick();
                p = 0;
            } else {
                p += 1;
            }
            rG.loop();

        }
        return 0;
    }


};

#endif //CITYOFWEIRDFISHES_SIMINITIALIZE_H

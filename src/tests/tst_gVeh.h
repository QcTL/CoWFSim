//
// Created by Laminar on 11/02/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GVEH_H
#define CITYOFWEIRDFISHES_TST_GVEH_H

#include <memory>
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBaseToPattern.h"
#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/roads/rNodeFromGrid.h"
#include "../display/menus/implementation/rBaseMenu.h"
#include "../sim/layers/implementations/sLayerType.h"
//#include "../sim/roads/rTransRNodeToRRNode.h"

int tst_gVeh() {
    std::shared_ptr<sMainSimulator> sMS = std::make_shared<sMainSimulator>(25);

    std::shared_ptr<gIGrid<uint8_t>> gB = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(25, 25, 0));
    std::shared_ptr<gIGrid<uint8_t>> gTransit = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(25, 25, 0));

    /*
    gB->set(0, 2, 2);
    gB->set(1, 0, 2);
    gB->set(1, 1, 2);
    gB->set(1, 2, 2);
    gB->set(1, 3, 2);
    gB->set(2, 2, 2);
*/

    gBaseToPattern<uint8_t> gBP(gB,
                                 gBaseToPattern<uint8_t>::gPatternType::gBPBlobSquares,
                                 gBaseToPattern<uint8_t>::gPatternParameters(3, 3, 6, 6),
                                 2);

    sMS->gLayerTypeGen = gB;
    sMS->gLayerAirPollution = gB;
    sMS->completedStartGrid();


    sMS->gLayerRoads[0][0]->refCompressed->addNewCar(sMS->gLayerRoads[12][9]->refCompressed->locIdNode,
                                                     sMS->gLayerRoads[12][9]->refCompressed->rBlock);

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
    int p = 0;
    while (rG.isOpen) {
        if( p > 4000) {
            sMS->tick();
            p = 0;
            std::cout<< "STEP" << std::endl;
        }else{
            p += 1;
        }
        rG.loop();
    }
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GVEH_H

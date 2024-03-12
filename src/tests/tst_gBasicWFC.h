//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICWFC_H
#define CITYOFWEIRDFISHES_TST_GBASICWFC_H

#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"
#include "../sim/structure/grids/transformation/gBaseToWFC.h"
#include "../sim/sMainSimulator.h"
#include "../display/menus/implementation/rBaseMenu.h"

int tst_gBasicWFC() {

    std::shared_ptr<sMainSimulator> sMS = std::make_shared<sMainSimulator>(105);
    std::shared_ptr<gIGrid<int>> gB =std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(30, 30, -1));

    std::vector<gtmElement> vElem = {
            gtmElement(0.0, 0, 0.0, 0),
            gtmElement(0.1, 0, 0.05, 0),
            gtmElement(0.15, 0, 0.3, 0),};

    gBaseToGradientMinimum gBGM(vElem, {15, 15}, gB, -1); // Pass ptr by reference
    gBGM.generateV2();

    std::cout << *dynamic_cast<gBasicGrid<int> *>(gB.get()) << std::endl;
    std::cout << "------------" << std::endl;

    //MASK:
    std::shared_ptr<gIGrid<bool>> b = BasicTransformations::genMaskFromGrid(gB, {1,2});

    std::cout << *dynamic_cast<gBasicGrid<bool> *>(b.get()) << std::endl;
    std::cout << "------------" << std::endl;

    std::shared_ptr<gIGrid<uint8_t>> gReal =std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(30, 30, 0));
    gBaseToWFC wfc(gReal, {{1,3},{2,7},{3,6}},{15,15}, 1, b);

    sMS->gLayerAirPollution = gReal;

    std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(sMS->gLayerAirPollution,
                                                                       sMS->gLayerCurStruct, sMS->gLayerTransit);
    //MENUS
    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
    std::shared_ptr<rBaseMenu> rBasic = std::make_shared<rBaseMenu>(rBaseMenu(pPM, sMS->gLayerTypeGen,
                                                                              sMS->gLayerRoads,
                                                                              sMS->sComp->gLayerOwnership, sMS->sComp->sTComp));
    pPM->addMenuTop(rBasic);
    rGlobal rG(gSimL, pPM);
    rG.setUp();
    while (rG.isOpen) {
        rG.loop();
        sMS->tick();
    }
    return 0;
}
#endif //CITYOFWEIRDFISHES_TST_GBASICWFC_H

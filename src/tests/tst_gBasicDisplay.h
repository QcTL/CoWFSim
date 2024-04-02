//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H
#define CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H

#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../display/layers/gDispLayers.h"
#include "../display/rGlobal.h"
#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/sSimulatorMain.h"
#include "../sim/structure/grids/transformation/sGridToSimulator.h"
#include "../display/menus/implementation/rBaseMenu.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"

int tst_gBasicDisplay() {

    std::shared_ptr<gIGrid<uint8_t>> gB =std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(30, 30, 255));
    std::shared_ptr<sSimulatorMain> sMS = std::make_shared<sSimulatorMain>(30);

    std::vector<gtmElement> vElem = {
            gtmElement(0.0, 0, 0.0, 0),
            gtmElement(0.5, 0, 0.05, 0),
            gtmElement(0.15, 0, 0.3, 0),};

    gBaseToGradientMinimum<uint8_t> gBGM(vElem, {15, 15}, gB, 255); // Pass ptr by reference
    gBGM.generateV2();

    std::cout << *dynamic_cast<gBasicGrid<uint8_t> *>(gB.get()) << std::endl;
    std::cout << "------------" << std::endl;

    sMS->gLayerAirPollution = gB;

    std::shared_ptr<gDispLayers> gSimL = std::make_shared<gDispLayers>(sMS->gLayerAirPollution,
                                                                       sMS->gMainTerrain->gLayerCurStruct, sMS->gLayerTransit);

    //MENUS
    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
    std::shared_ptr<rBaseMenu> rBasic = std::make_shared<rBaseMenu>(rBaseMenu(pPM, sMS->gLayerTypeGen,
                                                                              sMS->gLayerRoads,
                                                                              sMS->sSM_sCompany->gLayerOwnership, sMS->sSM_sCompany->sTComp));
    pPM->addMenuTop(rBasic);
    rGlobal rG(gSimL, pPM);
    rG.setUp();
    while (rG.isOpen) {
        rG.loop();
        sMS->tick();
    }
    return 0;
}
#endif //CITYOFWEIRDFISHES_TST_GBASICDISPLAY_H

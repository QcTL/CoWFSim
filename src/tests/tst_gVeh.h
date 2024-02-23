//
// Created by Laminar on 11/02/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GVEH_H
#define CITYOFWEIRDFISHES_TST_GVEH_H

#include <memory>
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBaseToPattern.h"
#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../display/layers/gSimLayers.h"
#include "../display/rGlobal.h"
#include "../sim/roads/rNodeFromGrid.h"
#include "../sim/roads/rTransRNodeToRRNode.h"

int tst_gVeh() {
    std::shared_ptr<gIGrid<int>> gB = std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(10, 10, 0));
    std::shared_ptr<gIGrid<int>> gTransit = std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(10, 10, 0));

    gB->set(0,2, 1);
    gB->set(1,0, 1);
    gB->set(1,1, 1);
    gB->set(1,2, 1);
    gB->set(1,3, 1);
    gB->set(2,2, 1);



    std::vector<rNode*> r = rNodeFromGrid<int>::givenGrid(gB, 1);
    rNode * rOne = r[0];
    std::list<std::shared_ptr<rRNodeI>> rLL = rTransRNodeToRRNode::conversion(rOne, 5, 10, gTransit);

    rInfoDist::initializeMatrix(10/5*10/5,5*5, rLL.size());

    //TODO tambe falta lo important que es tenir la matriu de la grid sapiguent a quin node estan "compactat".

    for (const std::shared_ptr<rRNodeI>& node : rLL) {
        node->sendInformationStart();
    }

    for(int i = 0; i < 3; i++) {
        for (const std::shared_ptr<rRNodeI>& node: rLL) {
            node->sendNewInformation();
        }
    }


    auto it = std::next(rLL.begin(), 2); // Iterator to the second element
    it->get()->addNewCar(1,0);

    auto it2 = std::next(rLL.begin(), 3); // Iterator to the second element
    it2->get()->addNewCar(1,0);


    rInfoDist::seeMatrix();
    std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gB));
    gLAP->setTransformation({0, 1, 2, 3, 4, 5});

    std::shared_ptr<gLayerTransit> gT = std::make_shared<gLayerTransit>(gLayerTransit(gTransit));
    gT->setTransformation({0, 1, 2, 3, 4, 5});


    std::shared_ptr<gSimLayers> gSimL = std::make_shared<gSimLayers>(gLAP, nullptr, gT, gB->rangeUse());
    gSimL->switchActual(gSimLayersTypes::G_TRANSIT);

    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
    std::shared_ptr<rBaseMenu> rBasic = std::make_shared<rBaseMenu>(
            pPM, rIMenu::rRelativePos::pBottomRight);
    pPM->addMenuTop(rBasic);
    rGlobal rG(gSimL,pPM);
    rG.setUp();
    int p = 0;

    while (rG.isOpen) {
        if( p > 2000) {
            for (const std::shared_ptr<rRNodeI> &node: rLL) {
                node->tick();
            }
            p = 0;
        }else{
            p += 1;
        }
        rG.loop();
    }

    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GVEH_H

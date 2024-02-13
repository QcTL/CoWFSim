//
// Created by Laminar on 11/02/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICPATTERN_H
#define CITYOFWEIRDFISHES_TST_GBASICPATTERN_H

#include <memory>
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBaseToPattern.h"
#include "../display/layers/implementation/gLayerAirPollution.h"
#include "../display/layers/gSimLayers.h"
#include "../display/rGlobal.h"
#include "../sim/roads/rNodeFromGrid.h"
#include "../sim/roads/rTransRNodeToRRNode.h"

int tst_gERCComm() {
    std::shared_ptr<gIGrid<int>> gB = std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(10, 10, 0));

    gB->set(0,1, 1);
    gB->set(1,1, 1);
    gB->set(2,1, 1);

    gB->set(1,0, 1);
    gB->set(1,2, 1);

    gB->set(0,3, 1);
    gB->set(1,3, 1);
    gB->set(2,3, 1);


    std::vector<rNode*> r = rNodeFromGrid<int>::givenGrid(gB, 1);
    rNode * rOne = r[0];
    std::list<rRNode*> rLL = rTransRNodeToRRNode::conversion(rOne, 5, 10);

    rInfoDist::initializeMatrix(10/5*10/5,5*5, rLL.size());

    //TODO tambe falta lo important que es tenir la matriu de la grid sapiguent a quin node estan "compactat".

    for (rRNode* node : rLL) {
        node->sendInformationStart();
    }

    for(int i = 0; i < 3; i++) {
        for (rRNode *node: rLL) {
            node->sendNewInformation();
        }
    }


    rInfoDist::seeMatrix();
    std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gB));
    gLAP->setTransformation({0, 1, 2, 3, 4, 5});
    std::shared_ptr<gSimLayers> gSimL = std::make_shared<gSimLayers>(gSimLayers(gLAP));

    std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>();
    rGlobal rG(gSimL,pPM);
    rG.setUp();
    while (rG.isOpen) {
        rG.loop();
    }

    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GBASICPATTERN_H

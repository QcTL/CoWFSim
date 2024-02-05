//
// Created by Laminar on 30/01/2024.
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

int tst_gExtractRoadsCompressed() {
    std::shared_ptr<gIGrid<int>> gB = std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(50, 50, 0));

    gB->set(0,0, 1);
    gB->set(1,0, 1);
    gB->set(2,0, 1);
    gB->set(3,0, 1);
    gB->set(4,0, 1);

    gB->set(2,1, 1);
    gB->set(4,1, 1);

    gB->set(2,2, 1);
    gB->set(3,2, 1);
    gB->set(4,2, 1);
    gB->set(5,2, 1);

    std::vector<rNode> r = rNodeFromGrid<int>::givenGrid(gB, 1);
    rNode * rOne = &r[0];
    rRNode * p = rTransRNodeToRRNode::conversion(rOne);

    std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gB));
    gLAP->setTransformation({0, 1, 2, 3, 4, 5});
    std::shared_ptr<gSimLayers> gSimL = std::make_shared<gSimLayers>(gSimLayers(gLAP));



    rGlobal rG(gSimL);
    rG.setUp();
    while (rG.isOpen) {
        rG.loop();
    }

    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GBASICPATTERN_H

//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_TRANSRNODERRNODE_H
#define CITYOFWEIRDFISHES_TST_TRANSRNODERRNODE_H

#include <iostream>
#include "../sim/roads/node/rNode.h"
#include "../sim/roads/node/rRNode.h"
#include "../sim/roads/rTransRNodeToRRNode.h"

int tst_tRNodeRRNode()
{
    //BASIC;
    rNode rUnic;

    rRNode* r = rTransRNodeToRRNode::conversion(&rUnic);

    //BASIC 2:
    rNode rB1;
    rNode rB2;

    rB1.rTop = &rB2;

    rRNode* r2 = rTransRNodeToRRNode::conversion(&rB1);

    //BASIC 3:
    rNode rT1;
    rNode rT2;
    rNode rT3;
    rNode rT4;

    rT1.rTop = &rT2;
    rT1.rRight = &rT3;
    rT3.rRight = &rT4;

    rRNode* r3 = rTransRNodeToRRNode::conversion(&rT1);

    return 0;
}
#endif //CITYOFWEIRDFISHES_TST_TRANSRNODERRNODE_H

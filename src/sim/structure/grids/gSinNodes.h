//
// Created by ganymede on 2/25/24.
//

#ifndef CITYOFWEIRDFISHES_GSINNODES_H
#define CITYOFWEIRDFISHES_GSINNODES_H

#include "../../roads/node/rNode.h"

class gSinNodes {
public:
    static std::vector<std::vector<rNode*>> gNodes;

    static void setGrid(const std::vector<std::vector<rNode*>>& gN){
        gNodes = gN;
    }
};
std::vector<std::vector<rNode*>> gSinNodes::gNodes;

#endif //CITYOFWEIRDFISHES_GSINNODES_H

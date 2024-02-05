//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RNODE_H
#define CITYOFWEIRDFISHES_RNODE_H

#include "rRNode.h"

struct rNode{
    struct rNode* rLeft;
    struct rNode* rRight;
    struct rNode* rTop;
    struct rNode* rBottom;

    rRNode *refCompressed;
    // Constructor
    rNode() : rLeft(nullptr), rRight(nullptr), rTop(nullptr), rBottom(nullptr) {}
};

#endif //CITYOFWEIRDFISHES_RNODE_H

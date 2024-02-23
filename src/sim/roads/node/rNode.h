//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RNODE_H
#define CITYOFWEIRDFISHES_RNODE_H

#include "rRNode.h"

struct rNode{
    std::pair<uint32_t , uint32_t> rPos;

    struct rNode* rLeft;
    struct rNode* rRight;
    struct rNode* rTop;
    struct rNode* rBottom;

    std::shared_ptr<rRNodeI> refCompressed;
    // Constructor
    rNode(std::pair<uint32_t , uint32_t> nPos) : rLeft(nullptr), rRight(nullptr), rTop(nullptr), rBottom(nullptr), refCompressed(nullptr), rPos(nPos) {}
};

#endif //CITYOFWEIRDFISHES_RNODE_H

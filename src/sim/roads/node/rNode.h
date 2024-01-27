//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RNODE_H
#define CITYOFWEIRDFISHES_RNODE_H

struct rNode{
    struct rNode* rLeft;
    struct rNode* rRight;
    struct rNode* rTop;
    struct rNode* rBottom;

    // Constructor
    rNode() : rLeft(nullptr), rRight(nullptr), rTop(nullptr), rBottom(nullptr) {}
};

#endif //CITYOFWEIRDFISHES_RNODE_H

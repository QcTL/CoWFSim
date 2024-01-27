//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RRNODE_H
#define CITYOFWEIRDFISHES_RRNODE_H

#include <list>
#include <cstdint>
#include <memory>

struct rRNode {
    uint8_t nCompressed;
        //Represents how many roads this single node represents;

    struct rRNode *rLeft;
    struct rRNode *rRight;
    struct rRNode *rTop;
    struct rRNode *rBottom;

    explicit rRNode(uint8_t n);

private:
    std::list<std::pair<int, uint8_t>> rRoadPresOrd;
    std::list<int> rRoadObstructionsOrd;
};

rRNode::rRNode(uint8_t n) : nCompressed(n) {
    rLeft = nullptr;
    rRight = nullptr;
    rTop = nullptr;
    rBottom = nullptr;
}

#endif //CITYOFWEIRDFISHES_RRNODE_H

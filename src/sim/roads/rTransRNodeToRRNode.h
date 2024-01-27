//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RTRANSRNODETORRNODE_H
#define CITYOFWEIRDFISHES_RTRANSRNODETORRNODE_H

#include "node/rNode.h"
#include "node/rRNode.h"

class rTransRNodeToRRNode {
    //Transform Road Node into Road Reduced Node

private:

    static bool isOnlyOneSeq(rNode *aR) {
        return (aR->rBottom != nullptr) + (aR->rTop != nullptr) + (aR->rRight != nullptr) + (aR->rLeft != nullptr) == 1;
    }

    static rNode *onlyOneSeq(rNode *aR) {
        if (aR->rTop != nullptr)
            return aR->rTop;
        else if (aR->rBottom != nullptr)
            return aR->rBottom;
        else if (aR->rLeft != nullptr)
            return aR->rLeft;
        else
            return aR->rRight;
    }


    static rRNode *rConversation(rRNode *prevAR, rNode *aR, uint8_t nCons) {
        if (aR == nullptr) {
            return nullptr;
        } else if (isOnlyOneSeq(aR)) {
            return rConversation(prevAR, onlyOneSeq(aR), ++nCons);
        } else {
            auto *newAR = new rRNode(nCons+1);
            newAR->rLeft = rConversation(newAR, aR->rLeft, 0);
            newAR->rRight = rConversation(newAR, aR->rRight, 0);
            newAR->rTop = rConversation(newAR, aR->rTop, 0);
            newAR->rBottom = rConversation(newAR, aR->rBottom, 0);

            return newAR;
        }
    }

public:
    static rRNode *conversion(rNode *rRoot) {
        return rConversation(nullptr, rRoot, 0);
    }
};

#endif //CITYOFWEIRDFISHES_RTRANSRNODETORRNODE_H

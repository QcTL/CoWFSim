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

    static int nFollow(rNode *aR) {
        return (aR->rBottom != nullptr) + (aR->rTop != nullptr) + (aR->rRight != nullptr) + (aR->rLeft != nullptr);
    }

    static std::pair<int, rNode *> onlyOneSeq(rNode *aR, int dirFromPrev) {
        if (aR->rTop != nullptr && dirFromPrev != 0)
            return {2, aR->rTop};
        if (aR->rBottom != nullptr && dirFromPrev != 2)
            return {0, aR->rBottom};
        if (aR->rLeft != nullptr && dirFromPrev != 3)
            return {1, aR->rLeft};
        if (aR->rRight != nullptr && dirFromPrev != 1)
            return {3, aR->rRight};
        return {};
    }

    static rRNode *rConversation(rRNode *prevAR, int dirFromPrev, int prevDirMajor, rNode *aR, uint8_t nCons) {
        //Dir 0 top, 1 right, 2 bottom, 3 left , -1 Començant
        if (aR->refCompressed != nullptr)
            return aR->refCompressed;

        rNode *aFollow = onlyOneSeq(aR, dirFromPrev).second;
        int nextDir = onlyOneSeq(aR, dirFromPrev).first;
        if (nFollow(aR) > 2 || (nFollow(aR) == 2 && nFollow(aFollow) > 2) || (nFollow(aR) == 1 && dirFromPrev != -1)) {
            rRNode *newAR;
            if (aR->refCompressed == nullptr) {
                newAR = new rRNode(nCons);
                aR->refCompressed = newAR;
            } else {
                newAR = aR->refCompressed;
            }
            switch (prevDirMajor) {
                case 0:
                    newAR->rTop = prevAR;
                    break;
                case 1:
                    newAR->rRight = prevAR;
                    break;
                case 2:
                    newAR->rBottom = prevAR;
                    break;
                case 3:
                    newAR->rLeft = prevAR;
                    break;
            }

            if (aR->rTop != nullptr && dirFromPrev != 0) {
                newAR->rTop = rConversation(newAR, 2, 2, aR->rTop, 1);
            }
            if (aR->rRight != nullptr && dirFromPrev != 1) {
                newAR->rRight = rConversation(newAR, 3, 3, aR->rRight, 1);
            }
            if (aR->rBottom != nullptr && dirFromPrev != 2) {
                newAR->rBottom = rConversation(newAR, 0, 0, aR->rBottom, 1);
            }
            if (aR->rLeft != nullptr && dirFromPrev != 3) {
                newAR->rLeft = rConversation(newAR, 1, 1, aR->rLeft, 1);
            }

            return newAR;
        } else if ((nFollow(aR) == 1 && dirFromPrev == -1) || nFollow(aR) == 2) {
            rRNode *r = rConversation(prevAR, nextDir, prevDirMajor, aFollow, ++nCons);
            aR->refCompressed = aFollow->refCompressed;
            return r;
        }

        return nullptr;
    }

public:
    static rRNode *conversion(rNode *rRoot) {
        return rConversation(nullptr, -1, -1, rRoot, 1);
    }
};

#endif //CITYOFWEIRDFISHES_RTRANSRNODETORRNODE_H

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
    int nFollow(rNode *aR) {
        return (aR->rBottom != nullptr) + (aR->rTop != nullptr) + (aR->rRight != nullptr) + (aR->rLeft != nullptr);
    }

    std::pair<int, rNode *> onlyOneSeq(rNode *aR, int dirFromPrev) {
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


    std::shared_ptr<rRNodeI>
    rConversation(const std::shared_ptr<rRNodeI> &prevAR, int dirFromPrev, int prevDirMajor, rNode *aR, uint8_t nCons,
                  const std::shared_ptr<gIGrid<uint8_t>> &tTransit) {
        //Dir 0 top, 1 right, 2 bottom, 3 left , -1 Començant
        if (aR->refCompressed != nullptr)
            return aR->refCompressed;

        rNode *aFollow = onlyOneSeq(aR, dirFromPrev).second;
        int nextDir = onlyOneSeq(aR, dirFromPrev).first;
        if (nFollow(aR) > 2 || (nFollow(aR) == 2 && nFollow(aFollow) > 2) || (nFollow(aR) == 1 && dirFromPrev != -1)) {
            std::shared_ptr<rRNodeI> newAR;
            if (aR->refCompressed == nullptr) {
                std::pair<uint32_t, uint32_t> pComp = aR->rPos;
                if (nFollow(aR) > 2) {
                    newAR = std::make_shared<rRNodeC>(
                            rRNodeC((uint16_t) pComp.first / rSizeBlocs * (rSizeGrid / rSizeBlocs) +
                                    pComp.second / rSizeBlocs));
                } else {
                    newAR = std::make_shared<rRNodeL>(
                            rRNodeL((uint16_t) pComp.first / rSizeBlocs * (rSizeGrid / rSizeBlocs) +
                                    pComp.second / rSizeBlocs, nCons));
                }
                newAR->tTransit = tTransit;
                llNodes.push_back(newAR);
                aR->refCompressed = newAR;
                aR->refCompressed->addRefPos(aR->rPos);
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
                default:
                    break;
            }

            if (aR->rTop != nullptr && dirFromPrev != 0) {
                newAR->rTop = rConversation(newAR, 2, 2, aR->rTop, 1, tTransit);
            }
            if (aR->rRight != nullptr && dirFromPrev != 1) {
                newAR->rRight = rConversation(newAR, 3, 3, aR->rRight, 1, tTransit);
            }
            if (aR->rBottom != nullptr && dirFromPrev != 2) {
                newAR->rBottom = rConversation(newAR, 0, 0, aR->rBottom, 1, tTransit);
            }
            if (aR->rLeft != nullptr && dirFromPrev != 3) {
                newAR->rLeft = rConversation(newAR, 1, 1, aR->rLeft, 1, tTransit);
            }

            return newAR;
        } else if ((nFollow(aR) == 1 && dirFromPrev == -1) || nFollow(aR) == 2) {
            std::shared_ptr<rRNodeI> r = rConversation(prevAR, nextDir, prevDirMajor, aFollow, ++nCons, tTransit);
            aR->refCompressed = aFollow->refCompressed;
            aR->refCompressed->addRefPos(aR->rPos);
            return r;
        }

        return nullptr;
    }

    uint16_t rSizeBlocs;
    uint16_t rSizeGrid;
    std::list<std::shared_ptr<rRNodeI>> llNodes;
public:
    std::list<std::shared_ptr<rRNodeI>>
    conversion(rNode *rRoot, uint16_t rSizeB, uint16_t rSizeG, const std::shared_ptr<gIGrid<uint8_t>> &tTransit) {
        rSizeBlocs = rSizeB;
        rSizeGrid = rSizeG;
        llNodes.clear();
        rConversation(nullptr, -1, -1, rRoot, 1, tTransit);
        return llNodes;
    }
};

#endif //CITYOFWEIRDFISHES_RTRANSRNODETORRNODE_H

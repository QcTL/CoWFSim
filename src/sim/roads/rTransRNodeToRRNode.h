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
        if(aR->refCompressed != nullptr){
            return aR->refCompressed;
        }

        if(nFollow(aR) == 1){
            if(dirFromPrev == -1){
                rNode *aFollow = onlyOneSeq(aR, dirFromPrev).second;
                int nextDir = onlyOneSeq(aR, dirFromPrev).first;
                rRNode* r=  rConversation(prevAR, nextDir, prevDirMajor, aFollow, ++nCons);
                aR ->refCompressed = aFollow ->refCompressed;
                return r;
            }else {
                auto newAR = new rRNode(nCons);
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
                aR ->refCompressed = newAR;
                return newAR;
            }
        } else if (nFollow(aR) == 2) { // VOL DIR QUE TU POTS SER OPTIMITZADA
            //Per tant si tens un prevAR que pot ser o no valida per optmització,
            //Aconseguir el node que es l'altre dle que s'ha vingut.
            rNode *aFollow = onlyOneSeq(aR, dirFromPrev).second;
            int nextDir = onlyOneSeq(aR, dirFromPrev).first;
            if(nFollow(aFollow) > 2){
                //Hem acabat de expendir-nos
                auto newAR = new rRNode(nCons);
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
                aR ->refCompressed = newAR;
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
            }else {
                rRNode *r = rConversation(prevAR, nextDir, prevDirMajor, aFollow, ++nCons);
                aR->refCompressed = aFollow->refCompressed;
                return r;
            }
        } else { //L'anterior no ho sabem pero aquesta segur que no.
            //Aixó vol dir que tenim la seguent si o si de la anterior en la direcció que s'hagui aprofundit
            //Per tant hem de crear un altre rRNode Nou:
            rRNode *newAR;
            if (aR->refCompressed == nullptr) {
                newAR = new rRNode(nCons);
                aR->refCompressed = newAR;
            } else {
                newAR = aR->refCompressed;
            }
            switch (dirFromPrev) {
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
        }
        //Ara l'hem de propagar per totes les direccions que tens menys per la que t'acaben de benir
        //Aixó en teoria et farà set de les teves altres variables que tens disponibles sense realment tornar-ho.
    }

public:
    static rRNode *conversion(rNode *rRoot) {
        return rConversation(nullptr, -1, -1, rRoot, 1);
    }
};

#endif //CITYOFWEIRDFISHES_RTRANSRNODETORRNODE_H

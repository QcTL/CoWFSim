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

    static bool isOnlyTwoSeq(rNode *aR) {
        return (aR->rBottom != nullptr) + (aR->rTop != nullptr) + (aR->rRight != nullptr) + (aR->rLeft != nullptr) >= 1;
    }

    static rNode *onlyOneSeq(rNode *aR, int dirFromPrev) {

        switch (dirFromPrev) {
            case 0:
                if (aR->rBottom != nullptr)
                    return aR->rBottom;
                else if (aR->rLeft != nullptr)
                    return aR->rLeft;
                else
                    return aR->rRight;
            case 1:
                if (aR->rTop != nullptr)
                    return aR->rTop;
                else if (aR->rBottom != nullptr)
                    return aR->rBottom;
                else if (aR->rLeft != nullptr)
                    return aR->rLeft;
            case 2:
                if (aR->rTop != nullptr)
                    return aR->rTop;
                else if (aR->rLeft != nullptr)
                    return aR->rLeft;
                else
                    return aR->rRight;
            case 3:
                if (aR->rTop != nullptr)
                    return aR->rTop;
                else if (aR->rBottom != nullptr)
                    return aR->rBottom;
                else
                    return aR->rRight;
            default:
                if (aR->rTop != nullptr)
                    return aR->rTop;
                else if (aR->rBottom != nullptr)
                    return aR->rBottom;
                else if (aR->rLeft != nullptr)
                    return aR->rLeft;
                else
                    return aR->rRight;
        }


    }


    static rRNode *rConversation(rRNode *prevAR,int dirFromPrev, rNode *aR, uint8_t nCons) {
        //Dir 0 top, 1 right, 2 bottom, 3 left , -1 Començant

        if(isOnlyTwoSeq(aR)){ // VOL DIR QUE TU POTS SER OPTIMITZADA
            //Per tant si tens un prevAR que pot ser o no valida per optmització,
            //Aconseguir el node que es l'altre dle que s'ha vingut.
            rNode *aFollow = onlyOneSeq(aR, dirFromPrev);
            int nextDir = 0;


            return rConversation(prevAR, nextDir, aFollow, ++nCons);

        }else{ //L'anterior no ho sabem pero aquesta segur que no.
            //Aixó vol dir que tenim la seguent si o si de la anterior en la direcció que s'hagui aprofundit
            //Per tant hem de crear un altre rRNode Nou:
            rRNode *newAR;
            if(aR->refCompressed == nullptr){
                newAR= new rRNode(0);
                aR->refCompressed = newAR;
            }else{
                newAR = aR->refCompressed;
            }
            switch(dirFromPrev){
                case 1:
                    newAR->rTop = prevAR;
                    break;
                case 2:
                    newAR->rBottom = prevAR;
                    break;
                case 3:
                    newAR->rRight = prevAR;
                    break;
                case 4:newAR->rLeft = prevAR;
                    break;
            }

            if(aR->rTop != nullptr && dirFromPrev != 0){
                newAR->rTop = rConversation(newAR, 0, aR->rTop, 0);
            }
            if(aR->rRight != nullptr  && dirFromPrev != 1){
                newAR->rRight = rConversation(newAR, 1, aR->rTop, 0);
            }
            if(aR->rBottom != nullptr && dirFromPrev != 2){
                newAR->rBottom = rConversation(newAR, 2, aR->rTop, 0);
            }
            if(aR->rLeft != nullptr  && dirFromPrev != 3){
                newAR->rLeft = rConversation(newAR, 3, aR->rTop, 0);
            }

            return newAR;
        }
        //Ara l'hem de propagar per totes les direccions que tens menys per la que t'acaben de benir
        //Aixó en teoria et farà set de les teves altres variables que tens disponibles sense realment tornar-ho.
    }

public:
    static rRNode *conversion(rNode *rRoot) {
        return rConversation(nullptr, rRoot, 0);
    }
};

#endif //CITYOFWEIRDFISHES_RTRANSRNODETORRNODE_H

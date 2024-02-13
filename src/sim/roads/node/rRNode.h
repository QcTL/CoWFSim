//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RRNODE_H
#define CITYOFWEIRDFISHES_RRNODE_H

#include <list>
#include <cstdint>
#include <memory>
#include "RNodeAttrs/rRMailbox.h"
#include "../transportation/gUniqueId.h"
#include "rInfoDists.h"

struct rRNode {
    uint8_t nCompressed;
    //Represents how many roads this single node represents;

    uint32_t uidNode;
    bool isDecision;
    uint16_t rBlock;
    rRMailbox rMailBox;

    struct rRNode *rLeft;
    struct rRNode *rRight;
    struct rRNode *rTop;
    struct rRNode *rBottom;

    explicit rRNode(uint8_t n, bool isDecision, uint16_t nBlock);

    void receive(const rRMail &r, const uint8_t &dir) {
        if (!isDecision) { //Si no es decisio i te un altre a l'altre costat
            //Enviar-ho al seguent si en tens;
            // amb la direccio canviada
            std::pair<uint8_t, rRNode *> sec = otherDir(dir);
            if(sec.second != nullptr) {
                rRMail rNew(r.mGridOrigin, r.mRoadStart, r.mSizePath + 1);
                sec.second->receive(rNew, sec.first);
            }
        } else {
            //Enviar-ho a rRMailbox;
            rMailBox.rMB.emplace(r, dir);
        }
    }

    void sendNewInformation() {
        //check if the destination of any position inside the grid or some other grid is lower that what we had.
        if(isDecision) {
            while (!rMailBox.rMB.empty()) {
                rRMail e = rMailBox.rMB.front().first;
                if (rInfoDist::addIfShorter(e, uidNode, rBlock, rMailBox.rMB.front().second)) {
                    // If true send the information to all the other elements in the other receiving end. + 1 in the dist
                    rRMail eN(e.mGridOrigin, e.mRoadStart, e.mSizePath + 1);
                    sendToNext(eN);
                }
                rMailBox.rMB.pop();
            }
        }
        //Set that if you enter this intersection and want to go to another one, just go for the one you received.
    }

    void sendInformationStart(){
        rInfoDist::addSelfDist(uidNode);
        rRMail eN(rBlock, uidNode, 1); // No es uidNode, es la posicio relativa de la carretera dins de la grid;
        sendToNext(eN);
    }

private:
    std::list<std::pair<int, uint8_t>> rRoadPresOrd;
    std::list<int> rRoadObstructionsOrd;

    /// COMMUNICATIONS //

    void sendToNext(const rRMail& m){
        if (rTop != nullptr) {
            sendDirection(m, 0b00000000);
        }
        if (rRight != nullptr) {
            sendDirection(m, 0b00000001);
        }
        if (rBottom != nullptr) {
            sendDirection(m, 0b00000010);
        }
        if (rLeft != nullptr) {
            sendDirection(m, 0b00000011);
        }
    }

    void sendDirection(rRMail mSend, uint8_t dDir) {
        uint8_t lastTwoBits = dDir & 0b11;
        rRNode *direction = getByDir(lastTwoBits);

        if(direction != nullptr) {
            direction->receive(mSend, dDir ^ 0x02);
            //The dir is changed to reflect the direction the reciving end is comming from
        }
    }

    std::pair<uint8_t, rRNode *> otherDir(uint8_t dirFromPrev) {
        if (rTop != nullptr && dirFromPrev != 0)
            return {2, rTop};
        if (rBottom != nullptr && dirFromPrev != 2)
            return {0, rBottom};
        if (rLeft != nullptr && dirFromPrev != 3)
            return {1, rLeft};
        if (rRight != nullptr && dirFromPrev != 1)
            return {3, rRight};
        return {};
    }

    rRNode* getByDir(const uint8_t n) const{
        switch (n) {
            case 0b00:
                return rTop;
            case 0b01:
                return rRight;
            case 0b10:
                return rBottom;
            case 0b11:
                return rLeft;
            default:
                return nullptr;
        }
    }

};

rRNode::rRNode(uint8_t n, bool isDecision, uint16_t nBlock) : nCompressed(n), isDecision(isDecision), rBlock(nBlock) {
    uidNode = gUniqueId::gen(nBlock);
    rLeft = nullptr;
    rRight = nullptr;
    rTop = nullptr;
    rBottom = nullptr;
}

#endif //CITYOFWEIRDFISHES_RRNODE_H

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
#include "../transportation/rActiveVehicles.h"
#include "rInfoDists.h"


class rRNodeI {
public:
    explicit rRNodeI(uint16_t rBlock) : uidNode(gUniqueId::gen(rBlock)), rBlock(rBlock), rLeft(nullptr),
                                        rRight(nullptr),
                                        rTop(nullptr), rBottom(nullptr) {}

    virtual void receive(const rRMail &r, const uint8_t &dir) = 0;

    virtual void sendNewInformation() = 0;

    virtual void tick() = 0;

    virtual std::pair<uint32_t, uint32_t> takeCarPrep(const uint8_t &dDir) = 0;

    virtual void addNewCar(uint32_t idDest, uint16_t blockDest) = 0;

    void sendInformationStart() {
        rInfoDist::addSelfDist(uidNode);
        rRMail eN(rBlock, uidNode, 1); // No es uidNode, es la posicio relativa de la carretera dins de la grid;
        sendToNext(eN);
    }

    std::shared_ptr<rRNodeI> rLeft;
    std::shared_ptr<rRNodeI> rRight;
    std::shared_ptr<rRNodeI> rTop;
    std::shared_ptr<rRNodeI> rBottom;

protected:
    uint32_t uidNode;
    uint16_t rBlock;

    virtual void enterCar(const uint8_t &dDir) = 0;

    void sendToNext(const rRMail &m) {
        for (int dir = 0; dir < 4; ++dir) {
            if ((rTop != nullptr && dir == 0) ||
                (rRight != nullptr && dir == 1) ||
                (rBottom != nullptr && dir == 2) ||
                (rLeft != nullptr && dir == 3)) {
                sendDirection(m, dir);
            }
        }
    }

    void sendDirection(rRMail mSend, uint8_t dDir) {
        uint8_t lastTwoBits = dDir & 0b11;
        std::shared_ptr<rRNodeI> direction = getByDir(lastTwoBits);
        if (direction != nullptr)
            direction->receive(mSend, dDir ^ 0x02); // Dir is the perspective of the receiver;
    }

    [[nodiscard]] std::shared_ptr<rRNodeI> getByDir(const uint8_t n) const {
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

    void notifyEnterNext(const uint8_t &dDir, const std::pair<uint32_t, uint32_t> &cNext) {
        uint8_t lastTwoBits = dDir & 0b11;
        std::shared_ptr<rRNodeI> direction = getByDir(lastTwoBits);
        if (direction != nullptr)
            direction->enterCar(dDir ^ 0x02);
    }
};


class rRNodeC : public rRNodeI {
    //CROSSING
public:
    rRNodeC(uint16_t rBlock) : rRNodeI(rBlock), itsEmpty(true) {}

    void receive(const rRMail &r, const uint8_t &dir) override {
        rMailBox.rMB.emplace(r, dir);
    }

    void sendNewInformation() override {
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

    void tick() override {
        if (itsEmpty && !reqTakeCar.empty()) {
            uint8_t dToTake = reqTakeCar.front();
            reqTakeCar.pop_front();
            carActInside = getByDir(dToTake)->takeCarPrep(dToTake ^ 0x02);

            notifyEnterNext(
                    rInfoDist::returnDirToDist(
                            rActiveVehicle::getDestByCar(carActInside.first).first,
                            rActiveVehicle::getDestByCar(carActInside.first).second,
                            rBlock, uidNode), carActInside);
        }
    }

    std::pair<uint32_t, uint32_t> takeCarPrep(const uint8_t &dDir) override {
        itsEmpty = true;
        return carActInside;
    }

    void addNewCar(uint32_t idDest, uint16_t blockDest) override{
    }


private:
    void enterCar(const uint8_t &dDir) override {
        /*
        if (itsEmpty) {
            carActInside = getByDir(dDir)->takeCarPrep(dDir ^ 0x02);
            itsEmpty = false;
            notifyEnterNext(
                    rInfoDist::returnDirToDist(
                            rActiveVehicle::getDestByCar(carActInside.first).first,
                            rActiveVehicle::getDestByCar(carActInside.first).second,
                            rBlock, uidNode), carActInside);
        } else*/
        reqTakeCar.push_back(dDir);
    }

    std::list<uint8_t> reqTakeCar;

    std::pair<uint32_t, uint32_t> carActInside;
    bool itsEmpty;
    rRMailbox rMailBox;
};

class rRNodeL : public rRNodeI {
    //LINE
public:

    rRNodeL(uint16_t rBlock, uint8_t nCompressed) :
            rRNodeI(rBlock), nCompressed(nCompressed), dFirst(nCompressed), dSecond(nCompressed) {
        std::vector<bool> elements(10, true);
    }

    void receive(const rRMail &r, const uint8_t &dir) override {
        std::pair<uint8_t, std::shared_ptr<rRNodeI>> sec = otherDir(dir);
        if (sec.second != nullptr) {
            rRMail rNew(r.mGridOrigin, r.mRoadStart, r.mSizePath + 1);
            sec.second->receive(rNew, sec.first);
        }
    }

    void sendNewInformation() override {
        //TODO CHANGE IT BECAUSE IT CANNOT BE IN THE CONSTRUCTOR BECAUSE NOT ALL THE NEIGHTBORES ARE SET;
        dEndFirst = getDirEntrances().first;
        dEndSecond = getDirEntrances().second;
    }

    void tick() override {
        for (auto it = dFirst.lOrderedCars.begin(); it != dFirst.lOrderedCars.end();) {
            auto& c = *it;
            if (c.second + 1 >= nCompressed) {
                if (rActiveVehicle::getDestByCar(c.first).first == uidNode &&
                    rActiveVehicle::getDestByCar(c.first).second == rBlock) {
                    it = dFirst.lOrderedCars.erase(it);
                    std::cout << "EL COTXE A ARRIBAT A LA SEVA DESTINACIO " << uidNode << "-" << rBlock << std::endl;
                } else {
                    notifyEnterNext(getBiggestOther().first, c);
                    ++it;
                }
            } else if (!dFirst.pState[c.second + 1]) {
                // Update state
                dFirst.pState[c.second] = false;
                dFirst.pState[c.second + 1] = true;
                ++c.second;
                ++it;
            } else {
                ++it;
            }
        }

        for (auto it = dSecond.lOrderedCars.begin(); it != dSecond.lOrderedCars.end();) {
            auto& c = *it;
            if (c.second + 1 >= nCompressed) {
                if (rActiveVehicle::getDestByCar(c.first).first == uidNode &&
                    rActiveVehicle::getDestByCar(c.first).second == rBlock) {
                    it = dSecond.lOrderedCars.erase(it);
                    std::cout << "EL COTXE A ARRIBAT A LA SEVA DESTINACIO " << uidNode << "-" << rBlock << std::endl;
                } else {
                    notifyEnterNext(getBiggestOther().first, c);
                    ++it;
                }
            } else if (!dSecond.pState[c.second + 1]) {
                // Update state
                dSecond.pState[c.second] = false;
                dSecond.pState[c.second + 1] = true;
                ++c.second;
                ++it;
            } else {
                ++it;
            }
        }
    }

    void addNewCar(uint32_t idDest, uint16_t blockDest) override{
        std::pair<uint32_t, uint32_t> newCar = {rActiveVehicle::addCar(idDest, blockDest), 0};
        //TODO CONTROLAR LA DIRECCIO INICIAL;
        dFirst.pState[0] = true;
        dFirst.lOrderedCars.push_back(newCar);
    }

private:

    std::pair<uint32_t, uint32_t> takeCarPrep(const uint8_t &dDir) override {
        if (dDir == dEndFirst) {
            //REMOVE FROM FIRST
            dFirst.pState[dFirst.pState.size()-1] = false;
            std::pair<uint32_t , uint32_t> cRet = dFirst.lOrderedCars.front();
            dFirst.lOrderedCars.pop_front();
            cRet.second = 0;
            return cRet;
        } else if (dDir == dEndSecond) {
            //REMOVE FROM SECOND
            dSecond.pState[dSecond.pState.size()-1] = false;
            std::pair<uint32_t , uint32_t> cRet = dSecond.lOrderedCars.front();
            dSecond.lOrderedCars.pop_front();
            return cRet;
        };
    }

    void enterCar(const uint8_t &dDir) override {
        std::pair<uint32_t, uint32_t> cNext = getByDir(dDir)->takeCarPrep(dDir ^ 0x02);
        if (isTheSmallestEntrance(dDir) && !dFirst.pState[0]) {
            dFirst.pState[0] = true;
            dFirst.lOrderedCars.push_back(cNext);
        } else if (!dSecond.pState[0]) {
            dSecond.pState[0] = true;
            dSecond.lOrderedCars.push_back(cNext);
        }
    }

    uint8_t nCompressed;

    struct rRoad {
        explicit rRoad(uint8_t nCompressed) : pState(nCompressed, false) {}

        std::vector<bool> pState;
        std::list<std::pair<uint32_t, uint32_t>> lOrderedCars;
    };

    rRoad dFirst;
    uint8_t dEndFirst;
    rRoad dSecond;
    uint8_t dEndSecond;

    std::pair<uint8_t, uint8_t> getDirEntrances() {
        uint8_t pMin = 5;
        uint8_t pMax = 0;
        if (rLeft != nullptr)
            pMin = std::min((uint8_t) 3, pMin);
        pMax = std::min((uint8_t) 3, pMax);
        if (rBottom != nullptr)
            pMin = std::min((uint8_t) 2, pMin);
        pMax = std::min((uint8_t) 2, pMax);
        if (rRight != nullptr)
            pMin = std::min((uint8_t) 1, pMin);
        pMax = std::min((uint8_t) 1, pMax);
        if (rTop != nullptr)
            pMin = std::min((uint8_t) 0, pMin);
        pMax = std::min((uint8_t) 0, pMax);

        return {pMin, pMax};
    }


    bool isTheSmallestEntrance(const uint8_t &dDir) {
        uint8_t pMin = 5;
        if (rLeft != nullptr)
            pMin = std::min((uint8_t) 3, pMin);
        if (rBottom != nullptr)
            pMin = std::min((uint8_t) 2, pMin);
        if (rRight != nullptr)
            pMin = std::min((uint8_t) 1, pMin);
        if (rTop != nullptr)
            pMin = std::min((uint8_t) 0, pMin);

        return dDir == pMin;
    }

    std::pair<uint8_t, std::shared_ptr<rRNodeI>> otherDir(uint8_t dirFromPrev) {
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

    std::pair<uint8_t, std::shared_ptr<rRNodeI>> getSmallestOther() {
        if (rTop != nullptr)
            return {0, rTop};
        if (rRight != nullptr)
            return {1, rBottom};
        if (rBottom != nullptr)
            return {2, rLeft};
        if (rLeft != nullptr)
            return {3, rRight};
        return {};
    }

    std::pair<uint8_t, std::shared_ptr<rRNodeI>> getBiggestOther() {
        if (rLeft != nullptr)
            return {3, rTop};
        if (rBottom != nullptr)
            return {2, rBottom};
        if (rRight != nullptr)
            return {1, rLeft};
        if (rTop != nullptr)
            return {0, rRight};
        return {};
    }
};

#endif //CITYOFWEIRDFISHES_RRNODE_H

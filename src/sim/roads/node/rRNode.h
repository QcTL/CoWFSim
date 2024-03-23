//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RRNODE_H
#define CITYOFWEIRDFISHES_RRNODE_H

#include <list>
#include <cstdint>
#include <memory>
#include <algorithm>
#include "RNodeAttrs/rRMailbox.h"
#include "../transportation/gUniqueId.h"
#include "../transportation/rActiveVehicles.h"
#include "rInfoDists.h"
#include "../../../display/rRemoteUpdateGrid.h"
#include "../../structure/grids/gIGrid.h"


class rRNodeI {
public:
    explicit rRNodeI(uint16_t rBlock)
            : locIdNode(gUniqueId::gen(rBlock)), globIdNode(gUniqueId::genGlob()), rBlock(rBlock), rRefPos({}),
              rLeft(nullptr),
              rRight(nullptr),
              rTop(nullptr),
              rBottom(nullptr) {}

    virtual void receive(const rRMail &r, const uint8_t &dir) = 0;

    virtual void sendNewInformation() = 0;

    virtual void tick() = 0;

    virtual std::pair<uint32_t, uint32_t> takeCarPrep(const uint8_t &dDir, const uint8_t &nLine) = 0;

    virtual void addNewCar(uint32_t idDest, uint16_t blockDest) = 0;

    void sendInformationStart() {
        rInfoDist::addSelfDist(locIdNode);
        rRMail eN(rBlock, locIdNode, 1); // No es locIdNode, es la posicio relativa de la carretera dins de la grid;
        sendToNext(eN);
    }

    [[nodiscard]] virtual float getOccupancy() const = 0;

    virtual int getCapacity() = 0;

    [[nodiscard]] virtual std::vector<std::list<uint32_t>> getPosRoad(int idRoad) const = 0;

    virtual bool isCrossing() = 0;

    virtual std::pair<std::pair<int, int>, std::pair<int, int>> getSizesNeighbor() {
        return {{rTop == nullptr ? -1 : 1,  rBottom == nullptr ? -1 : 1},
                {rLeft == nullptr ? -1 : 1, rRight == nullptr ? -1 : 1}};
    }

    std::shared_ptr<rRNodeI> rLeft;
    std::shared_ptr<rRNodeI> rRight;
    std::shared_ptr<rRNodeI> rTop;
    std::shared_ptr<rRNodeI> rBottom;

    void addRefPos(const std::pair<uint32_t, uint32_t> &pNew) {
        rRefPos.push_back(pNew);
    }

    std::shared_ptr<gIGrid<uint8_t>> tTransit;
    uint32_t locIdNode;
    uint32_t globIdNode;
    uint16_t rBlock;
protected:

    virtual void enterCar(const uint8_t &dDir, const uint8_t &nLine) = 0;

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

    void notifyEnterNext(const uint8_t &dDir, const std::pair<uint32_t, uint32_t> &cNext, uint8_t nLine) {
        uint8_t lastTwoBits = dDir & 0b11;
        std::shared_ptr<rRNodeI> direction = getByDir(lastTwoBits);
        if (direction != nullptr)
            direction->enterCar(dDir ^ 0x02, nLine);
    }

    void updateRefGrid(uint8_t nValue) {
        for (const auto &p: rRefPos) {
            tTransit->set(p, nValue + 1);
        }
        rRemoteUpdateGrid::setHasToChange(true);
    }

private:
    std::vector<std::pair<uint32_t, uint32_t>> rRefPos;
};

class rRNodeC : public rRNodeI {
    //CROSSING
public:
    explicit rRNodeC(uint16_t rBlock) : rRNodeI(rBlock), itsEmpty(true) {}

    bool isCrossing() override { return true; }

    void receive(const rRMail &r, const uint8_t &dir) override {
        rMailBox.rMB.emplace(r, dir);
    }

    void sendNewInformation() override {
        while (!rMailBox.rMB.empty()) {
            rRMail e = rMailBox.rMB.front().first;
            if (rInfoDist::addIfShorter(e, globIdNode, rBlock, rMailBox.rMB.front().second)) {
                rRMail eN(e.mGridOrigin, e.mRoadStart, e.mSizePath + 1);
                sendToNext(eN);
            }
            rMailBox.rMB.pop();
        }
    }

    void tick() override {
        if (!itsEmpty) {
            if (rActiveVehicle::getDestByCar(carActInside.first).second == rBlock &&
                rActiveVehicle::getDestByCar(carActInside.first).first == locIdNode) {
                itsEmpty = true;
                updateRefGrid(false);
            } else {
                notifyEnterNext(
                        rInfoDist::returnDirToDist(
                                rActiveVehicle::getDestByCar(carActInside.first).first,
                                rActiveVehicle::getDestByCar(carActInside.first).second,
                                rBlock, globIdNode), carActInside, 0);
            }

        } else if (!reqTakeCar.empty()) {
            uint8_t dToTake = reqTakeCar.front().first;
            carActInside = getByDir(dToTake)->takeCarPrep(dToTake ^ 0x02, reqTakeCar.front().second);
            reqTakeCar.pop_front();
            itsEmpty = false;
            updateRefGrid(true);
        }

    }

    std::pair<uint32_t, uint32_t> takeCarPrep(const uint8_t &dDir, const uint8_t &nLine) override {
        itsEmpty = true;
        updateRefGrid(false);
        return carActInside;
    }

    void addNewCar(uint32_t idDest, uint16_t blockDest) override {}

    [[nodiscard]] float getOccupancy() const override { return itsEmpty ? 0 : 1.0; }

    int getCapacity() override { return 1; }

    [[nodiscard]] std::vector<std::list<uint32_t>> getPosRoad(int idRoad) const override {
        if (itsEmpty) return {{}};
        return {{0}};
    }

private:
    void enterCar(const uint8_t &dDir, const uint8_t &nLine) override {
        /*
        if (itsEmpty) {
            carActInside = getByDir(dDir)->takeCarPrep(dDir ^ 0x02);
            itsEmpty = false;
            notifyEnterNext(
                    rInfoDist::returnDirToDist(
                            rActiveVehicle::getDestByCar(carActInside.first).first,
                            rActiveVehicle::getDestByCar(carActInside.first).second,
                            rBlock, locIdNode), carActInside);
        } else*/
        reqTakeCar.emplace_back(dDir, nLine);
    }

    //Direcction, line
    std::list<std::pair<uint8_t, uint8_t>> reqTakeCar;

    std::pair<uint32_t, uint32_t> carActInside;
    bool itsEmpty;
    rRMailbox rMailBox;
};

class rRNodeL : public rRNodeI {
    //LINE
public:
    rRNodeL(uint16_t rBlock, uint8_t nCompressed, uint8_t nLines) :
            rRNodeI(rBlock), nCompressed(nCompressed), nLines(nLines) {
        dVecFirst = std::vector<rRoad>(nLines, rRoad(nCompressed));
        dVecSecond = std::vector<rRoad>(nLines, rRoad(nCompressed));
    }

    bool isCrossing() override { return false; }

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

    bool hasChanged = false;

    void tick() override {
        std::vector<std::vector<rRoad>> vTwoDirections = {dVecFirst, dVecSecond};

        for (auto &dVecAct: vTwoDirections) {
            for (int i = 0; i < nLines; i++) {
                for (auto it = dVecAct[i].lOrderedCars.begin(); it != dVecAct[i].lOrderedCars.end();) {
                    auto &c = *it;
                    if (c.second + 1 >= nCompressed) {
                        //Si el seguent ja es l'ultim
                        if (rActiveVehicle::getDestByCar(c.first).first == locIdNode &&
                            rActiveVehicle::getDestByCar(c.first).second == rBlock) {
                            //Si ha arribat a la seva destinacio
                            dVecAct[i].pState[c.second] = false;
                            it = dVecAct[i].lOrderedCars.erase(it);
                            hasChanged = true;
                            std::cout << "DESTINATION" << std::endl;
                        } else if (!dVecAct[i].hasRequestedNext) {
                            //Demana si s'ha fet una peticio al seguent;
                            dVecAct[i].hasRequestedNext = true;
                            notifyEnterNext(dEndFirst, c, i);
                            ++it;
                        } else if (!dVecAct[i].hasRequestedNext) {
                            //Si ho ha fet no fara res
                            ++it;
                        }
                    } else {
                        uint8_t nToMove = c.second % (c.first & 0xF) == 0 ? 2 : 1;
                        if (dVecAct[i].pState.size() > c.second + nToMove) {
                            if (i > 0 && !dVecAct[i - 1].pState[c.second + nToMove]) {
                                dVecAct[i].pState[c.second] = false;
                                dVecAct[i - 1].pState[c.second + nToMove] = true;
                                ++c.second;
                                ++it;
                            } else if (!dVecAct[i].pState[c.second + nToMove]) {
                                dVecAct[i].pState[c.second] = false;
                                dVecAct[i].pState[c.second + nToMove] = true;
                                ++c.second;
                                ++it;
                            } else if (nToMove == 2 && !dVecAct[i].pState[c.second + nToMove]) {
                                dVecAct[i].pState[c.second] = false;
                                dVecAct[i].pState[c.second + 1] = true;
                                ++c.second;
                                ++it;
                            } else {
                                ++it;
                            }
                        } else if (nToMove == 2 && dVecAct[i].pState.size() > c.second + 1) {
                            if (!dVecAct[i].pState[c.second + 1]) {
                                dVecAct[i].pState[c.second] = false;
                                dVecAct[i].pState[c.second + 1] = true;
                                ++c.second;
                                ++it;
                            }
                        } else {
                            ++it;
                        }
                    }
                }
            }
        }

        if (hasChanged) {
            hasChanged = false;
            updateRefGrid(getMaxTotalCarsRoad());
        }
    }

    void addNewCar(uint32_t idDest, uint16_t blockDest) override {
        std::pair<uint32_t, uint32_t> newCar = {rActiveVehicle::addCar(idDest, blockDest), 0};
        //Afegir en la primera carretera:
        dVecFirst[0].pState[0] = true;
        dVecFirst[0].lOrderedCars.push_back(newCar);
        hasChanged = true;
        updateRefGrid(getMaxTotalCarsRoad());
    }

    [[nodiscard]] uint8_t getMaxTotalCarsRoad() const {
        uint32_t sOccFirst = 0;
        uint32_t sOccSecond = 0;
        for (int i = 0; i < nLines; i++) {
            sOccFirst += dVecFirst[i].lOrderedCars.size();
            sOccSecond += dVecSecond[i].lOrderedCars.size();
        }
        return std::max(sOccFirst, sOccSecond);
    }

    [[nodiscard]] float getOccupancy() const override {
        uint32_t sOccMax = getMaxTotalCarsRoad();
        return (float) sOccMax / (float) nCompressed;
    }

    int getCapacity() override { return nCompressed; }

    [[nodiscard]] std::vector<std::list<uint32_t>> getPosRoad(int idRoad) const override {
        auto &road = (idRoad == 0) ? dVecFirst : dVecSecond;
        std::vector<std::list<uint32_t>> intList = std::vector<std::list<uint32_t>>(nLines);
        for (int i = 0; i < nLines; ++i)
            std::transform(road[i].lOrderedCars.begin(), road[i].lOrderedCars.end(),
                           std::back_inserter(intList[i]),
                           [](const auto &c) { return c.second; });
        return intList;
    }

private:

    std::pair<uint32_t, uint32_t> takeCarPrep(const uint8_t &dDir, const uint8_t &nLine) override {
        std::pair<uint32_t, uint32_t> cRet;
        // Remove from the appropriate data structure based on dDir
        if (dDir == dEndFirst) {
            // Remove from first
            if (!dVecFirst[nLine].lOrderedCars.empty()) {
                dVecFirst[nLine].pState[dVecFirst[nLine].pState.size() - 1] = false;
                cRet = dVecFirst[nLine].lOrderedCars.front();
                dVecFirst[nLine].lOrderedCars.pop_front();
                cRet.second = 0;
                dVecFirst[nLine].hasRequestedNext = false;
                hasChanged = true;
            }
        } else if (dDir == dEndSecond) {
            // Remove from second
            if (!dVecSecond[nLine].lOrderedCars.empty()) {
                dVecSecond[nLine].pState[dVecSecond[nLine].pState.size() - 1] = false;
                cRet = dVecSecond[nLine].lOrderedCars.front();
                dVecSecond[nLine].lOrderedCars.pop_front();
                cRet.second = 0;
                dVecSecond[nLine].hasRequestedNext = false;
                hasChanged = true;
            }
        }
        if (hasChanged) {
            hasChanged = false;
            updateRefGrid(getMaxTotalCarsRoad());
        }
        return cRet;
    }

    void enterCar(const uint8_t &dDir, const uint8_t &nLine) override {
        std::pair<uint32_t, uint32_t> cNext = getByDir(dDir)->takeCarPrep(dDir ^ 0x02, 0);
        if (dEndFirst != dDir && !dVecFirst[0].pState[0]) {
            dVecFirst[0].pState[0] = true;
            dVecFirst[0].lOrderedCars.push_back(cNext);
            hasChanged = true;
        } else if (!dVecSecond[0].pState[0]) {
            dVecSecond[0].pState[0] = true;
            dVecSecond[0].lOrderedCars.push_back(cNext);
            hasChanged = true;
        }

        if (hasChanged) {
            hasChanged = false;
            updateRefGrid(getMaxTotalCarsRoad());
        }
    }

    uint8_t nCompressed;
    uint8_t nLines;

    struct rRoad {
        explicit rRoad(uint8_t nCompressed) : pState(nCompressed, false) {}

        std::vector<bool> pState;
        std::list<std::pair<uint32_t, uint32_t>> lOrderedCars;
        bool hasRequestedNext = false;
    };

    std::vector<rRoad> dVecFirst;
    uint8_t dEndFirst;
    //bool hasRequestedFirst = false;

    //rRoad dSecond;
    std::vector<rRoad> dVecSecond;
    uint8_t dEndSecond;
    //bool hasRequestedSecond = false;

    std::pair<uint8_t, uint8_t> getDirEntrances() {
        std::vector<int> sVec;
        if (rLeft != nullptr)
            sVec.push_back(3);
        if (rBottom != nullptr)
            sVec.push_back(2);
        if (rRight != nullptr)
            sVec.push_back(1);
        if (rTop != nullptr)
            sVec.push_back(0);

        auto [minIt, maxIt] = std::minmax_element(sVec.begin(), sVec.end());
        return {maxIt != sVec.end() ? *maxIt : -1, minIt != sVec.end() ? *minIt : -1};
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
};

#endif //CITYOFWEIRDFISHES_RRNODE_H

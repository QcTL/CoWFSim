//
// Created by Laminar on 15/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SGUNDERGROUNDMAIN_H
#define CITYOFWEIRDFISHES_SGUNDERGROUNDMAIN_H

#include <cstdint>
#include <memory>
#include <vector>
#include <map>
#include <list>
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"

class sgUndergroundMain {
public:

    explicit sgUndergroundMain(uint32_t inGridSize) {
        gLayerUnderground = std::make_shared<gBasicGrid<uint8_t >>
                (gBasicGrid<uint8_t>(inGridSize, inGridSize, 0));
        sgUM_tActualStation = 0;
    }

    struct sgUM_gPosStation {
        std::pair<int, int> sPos;
        struct gRouteStation {
            sgUM_gPosStation *gNextStation;
            std::vector<std::pair<int, int>> gNexRoute;
        };
        std::vector<gRouteStation> gVecNext;
    };

    void setPointsTransit(const std::vector<std::vector<std::pair<int, int>>> &totalStations) {
        for (const auto &i: totalStations)
            sgUM_totalStations.push_back({i[0], {}});
        for (int i = 0; i < totalStations.size(); i++)
            sgUM_totalStations[i].gVecNext.push_back(
                    {&sgUM_totalStations[(i + 1) % sgUM_totalStations.size()], totalStations[i]});

        uint32_t _sStartTimeLoop = 0;
        uint32_t _prepTime = 0;
        for (int j = 0; j < sgUM_totalStations.size(); j++) {
            sgUM_timeArriving[j] = {};
        }

        while (_sStartTimeLoop < 288 * 5) { //Nombre de voltes
            for (int j = 0; j < sgUM_totalStations.size(); j++) {
                sgUM_timeArriving[j].push_back(_sStartTimeLoop + _prepTime);
                sgUM_vecTimeStation.emplace_back(_sStartTimeLoop + _prepTime, j);
                _prepTime += sgUM_totalStations[j].gVecNext[0].gNexRoute.size();

                if(_sStartTimeLoop + _prepTime > 288 * 5)
                    break;
            }
            _sStartTimeLoop += _prepTime;
            _prepTime = 0;
        }

        sgUM_actPos = sgUM_totalStations[0].sPos;
        sgUM_tActualStation = 0;
        gLayerUnderground->set(sgUM_actPos, gLayerUnderground->get(sgUM_actPos) + 32);
    }

    std::pair<int, int> getActualPosition(const uint32_t inCTime) {
        if (inCTime != sgUM_vecTimeStation[sgUM_tActualStation].first) {
            if (inCTime >= sgUM_vecTimeStation[sgUM_vecTimeStation.size() - 1].first)
                return sgUM_totalStations[sgUM_vecTimeStation[sgUM_tActualStation].second].sPos;

            //No esta en la estacio:
            if (inCTime >= sgUM_vecTimeStation[(sgUM_tActualStation + 1) % sgUM_vecTimeStation.size()].first) {
                //ES EL SEGUENT
                sgUM_tActualStation++;
            } else {
                //ESTA ENTRE LES DUES;
                uint32_t timeFromExitPrevious = inCTime - sgUM_vecTimeStation[sgUM_tActualStation].first;
                uint32_t stationExitPrevious = sgUM_vecTimeStation[sgUM_tActualStation].second;
                return sgUM_totalStations[stationExitPrevious].gVecNext[0].gNexRoute[timeFromExitPrevious];
            }
        }
        return sgUM_totalStations[sgUM_vecTimeStation[sgUM_tActualStation].second].sPos;
    }

    int hasStation(const std::pair<int,int>& inPStation){
        for(int i = 0; i < sgUM_totalStations.size(); i++)
            if(sgUM_totalStations[i].sPos.first == inPStation.first && sgUM_totalStations[i].sPos.second == inPStation.second)
                return i;

        return -1;
    }

    void tick(uint32_t inTick) {
        gLayerUnderground->set(sgUM_actPos, gLayerUnderground->get(sgUM_actPos) - 32);
        if (inTick == 0)
            sgUM_tActualStation = 0;

        sgUM_actPos = getActualPosition(inTick);
        gLayerUnderground->set(sgUM_actPos, gLayerUnderground->get(sgUM_actPos) + 32);
    }

    uint32_t getClosestTimeForStation(const uint16_t inNStation, const uint32_t inCTime) {
        for (const uint32_t tArr: sgUM_timeArriving[inNStation])
            if (tArr >= inCTime * 5)
                return tArr / 5;
        return 0;
    }

    uint32_t getPastClosestTimeForStation(const uint16_t inNStation, const uint32_t inCTime) {
        for (const uint32_t tArr: sgUM_timeArriving[inNStation])
            if (tArr <= inCTime * 5)
                return tArr / 5;
        return 0;
    }

    struct sgUM_lowestViableRoute {
        uint16_t closestSt1;
        uint16_t closestSt2;
        uint32_t totalDistance;
    };

    std::pair<int, int> getPosStationById(uint16_t uuidStation) {
        return sgUM_totalStations[uuidStation].sPos;
    }

    sgUM_lowestViableRoute
    getLowestDistanceCommute(const std::pair<int, int> inPoint1, const std::pair<int, int> inPoint2) {
        uint16_t _closestSt1 = 0, _closestSt2 = 0;
        uint32_t _dMinDistance1 = 0, _dMinDistance2 = 0;

        for (int i = 0; i < sgUM_totalStations.size(); i++) {
            uint32_t _dDist1 = (inPoint1.first - sgUM_totalStations[i].sPos.first) *
                               (inPoint1.first - sgUM_totalStations[i].sPos.first) +
                               (inPoint1.second - sgUM_totalStations[i].sPos.second) *
                               (inPoint1.second - sgUM_totalStations[i].sPos.second);
            uint32_t _dDist2 = (inPoint2.first - sgUM_totalStations[i].sPos.first) *
                               (inPoint2.first - sgUM_totalStations[i].sPos.first) +
                               (inPoint2.second - sgUM_totalStations[i].sPos.second) *
                               (inPoint2.second - sgUM_totalStations[i].sPos.second);
            if (_dMinDistance1 == 0 || _dDist1 < _dMinDistance1) {
                _closestSt1 = i;
                _dMinDistance1 = _dDist1;
            }

            if (_dMinDistance2 == 0 || _dDist2 < _dMinDistance2) {
                _closestSt2 = i;
                _dMinDistance2 = _dDist2;
            }
        }

        return {_closestSt1, _closestSt2, _dMinDistance1 + _dMinDistance2};
    }

    std::shared_ptr<gIGrid<uint8_t>> gLayerUnderground;
private:

    std::map<uint16_t, std::list<uint32_t>> sgUM_timeArriving; //Ordered List by Time;
    std::vector<sgUM_gPosStation> sgUM_totalStations;

    std::vector<std::pair<uint32_t, uint16_t>> sgUM_vecTimeStation;
    uint32_t sgUM_tActualStation;
    std::pair<int, int> sgUM_actPos;
};

#endif //CITYOFWEIRDFISHES_SGUNDERGROUNDMAIN_H

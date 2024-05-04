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

    /**
     * @fn void setPointsTransit
     * @brief This function set up the station stops that will be set up at the day given a vector of the total stations
     * @param totalStations A vector of points, that represents the path on each line to the next.
     */
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

    /**
     * @fn std::pair<int, int> getActualPosition
     * @brief Given the hour of the day returns a position where the metro is situated.
     * @param inCTime The reduced hour of the simulation 288
     * @return The position in a pair of integers of the metro
     */
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

    /**
     * @fn int hasStation
     * @param inPStation A position of positive coordinates
     * @return -1 if it doesn't have an station or the index of the station if it does
     */
    int hasStation(const std::pair<int,int>& inPStation){
        for(int i = 0; i < sgUM_totalStations.size(); i++)
            if(sgUM_totalStations[i].sPos.first == inPStation.first && sgUM_totalStations[i].sPos.second == inPStation.second)
                return i;

        return -1;
    }

    /**
     * @fn void tick
     * @brief Updates the elements in the class that need changing every tick of the simulation
     * @param inTick the number of ticks that occurred in this day of simulation
     */
    void tick(uint32_t inTick) {
        gLayerUnderground->set(sgUM_actPos, gLayerUnderground->get(sgUM_actPos) - 32);
        if (inTick == 0)
            sgUM_tActualStation = 0;

        sgUM_actPos = getActualPosition(inTick);
        gLayerUnderground->set(sgUM_actPos, gLayerUnderground->get(sgUM_actPos) + 32);
    }

    /**
     * @fn uint32_t getClosestTimeForStation
     * @param inNStation The index of the station, has to be lower that the maximum amount of stations
     * @param inCTime the compressed hour of that day in the simulation < 288
     * @return A compressed hour where the closest train passing throw that station comes
     */
    uint32_t getClosestTimeForStation(const uint16_t inNStation, const uint32_t inCTime) {
        for (const uint32_t tArr: sgUM_timeArriving[inNStation])
            if (tArr >= inCTime * 5)
                return tArr / 5;
        return 0;
    }

    struct sgUM_lowestViableRoute {
        uint16_t closestSt1;
        uint16_t closestSt2;
        uint32_t totalDistance;
    };

    /**
     * @fn std::pair<int, int> getPosStationById
     * @brief Return the position of any station given its index
     * @param uuidStation The index of the station, has to be lower that the maximum amount of stations
     * @return The positive coordinates of that station
     */
    std::pair<int, int> getPosStationById(uint16_t uuidStation) {
        return sgUM_totalStations[uuidStation].sPos;
    }

    /**
     * @fn sgUM_lowestViableRoute getLowestDistanceCommute
     * @brief Given two points, this function returns the shortest route in metro that passes throw that two points, and the
     * distance to the start and the nearest metro station and the distance of the end metro station and the intPoint2
     * @param inPoint1 A positive pair of coordinates of the first point
     * @param inPoint2 A positive pair of coordinates of the first point, cannot be the same as the first one
     * @return A struct containing all the information about the route
     */
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

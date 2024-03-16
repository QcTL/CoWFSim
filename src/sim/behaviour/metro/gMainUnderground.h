//
// Created by Laminar on 15/03/2024.
//

#ifndef CITYOFWEIRDFISHES_GMAINUNDERGROUND_H
#define CITYOFWEIRDFISHES_GMAINUNDERGROUND_H

#include <cstdint>
#include <memory>
#include <vector>
#include <map>
#include <list>
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"

class gMainUnderground {
public:

    gMainUnderground(uint32_t lSize) {
        gLayerUnderground = std::make_shared<gBasicGrid<uint8_t >>
                (gBasicGrid<uint8_t>(lSize, lSize, 0));
    }

    struct gPosStation {
        std::pair<int, int> sPos;
        struct gRouteStation {
            gPosStation *gNextStation;
            std::vector<std::pair<int, int>> gNexRoute;
        };
        std::vector<gRouteStation> gVectNext;
    };

    void setPointsTransit(const std::vector<std::vector<std::pair<int, int>>> &totalStations) {
        for (const auto &i: totalStations) {
            sTotalStations.push_back({i[0], {}});
        }
        for (int i = 0; i < totalStations.size(); i++) {
            sTotalStations[i].gVectNext.push_back({&sTotalStations[(i + 1) % sTotalStations.size()], totalStations[i]});
        }

        uint32_t sStartTimeLoop = 0;
        uint32_t prepTime = 0;
        for (int j = 0; j < sTotalStations.size(); j++) {
            sTimeArriving[j] = {};
        }

        for (int i = 0; i < 10; i++) { //Nombre de voltes
            for (int j = 0; j < sTotalStations.size(); j++) {
                sTimeArriving[j].push_back(sStartTimeLoop + prepTime);
                gVectorTimeStation.emplace_back(sStartTimeLoop + prepTime, j);
                prepTime += sTotalStations[j].gVectNext[0].gNexRoute.size();
            }
            sStartTimeLoop += prepTime;
            prepTime = 0;
        }

        pActualP = sTotalStations[0].sPos;
        tActualStation = 0;
        gLayerUnderground->set(pActualP, gLayerUnderground->get(pActualP) + 16);
    }

    std::pair<int, int> getActualPosition(const uint32_t tTime) {
        if (tTime != gVectorTimeStation[tActualStation].first) {
            //No esta en la estacio:
            if (tTime >= gVectorTimeStation[(tActualStation + 1) % gVectorTimeStation.size()].first) {
                //ES EL SEGUENT
                tActualStation++;
            } else {
                //ESTA ENTRE LES DUES;
                uint32_t timeFromExitPrevious = tTime - gVectorTimeStation[tActualStation].first;
                uint32_t stationExitPrevious = gVectorTimeStation[tActualStation].second;
                return sTotalStations[stationExitPrevious].gVectNext[0].gNexRoute[timeFromExitPrevious];
            }
        }
        return sTotalStations[gVectorTimeStation[tActualStation].second].sPos;
    }


    void tick(const uint32_t tTime) {
        gLayerUnderground->set(pActualP, gLayerUnderground->get(pActualP) - 16);

        pActualP = getActualPosition(tTime);
        gLayerUnderground->set(pActualP, gLayerUnderground->get(pActualP) + 16);
    }

    uint32_t getClosestTimeForStationArriving(const uint16_t nStation, const uint32_t tTime) {
        for (const uint32_t tArr: sTimeArriving[nStation])
            if (tArr > tTime)
                return tArr;
        return 0;
    }

    struct lowestViableRoute {
        uint16_t closestSt1;
        uint16_t closestSt2;
        uint32_t totalDistance;
    };

    lowestViableRoute getLowestDistanceCommute(const std::pair<int, int> p1, const std::pair<int, int> p2) {
        uint16_t closestSt1 = 0, closestSt2 = 0;
        uint32_t dMinDistance1 = 0, dMinDistance2 = 0;

        for (int i = 0; i < sTotalStations.size(); i++) {
            uint32_t dDist1 = (p1.first - sTotalStations[i].sPos.first) * (p1.first - sTotalStations[i].sPos.first) +
                              (p1.second - sTotalStations[i].sPos.second) * (p1.second - sTotalStations[i].sPos.second);
            uint32_t dDist2 = (p2.first - sTotalStations[i].sPos.first) * (p2.first - sTotalStations[i].sPos.first) +
                              (p2.second - sTotalStations[i].sPos.second) * (p2.second - sTotalStations[i].sPos.second);
            if (dMinDistance1 == 0 || dDist1 < dMinDistance1) {
                closestSt1 = i;
                dMinDistance1 = dDist1;
            }

            if (dMinDistance2 == 0 || dDist2 < dMinDistance2) {
                closestSt2 = i;
                dMinDistance2 = dDist2;
            }
        }

        return {closestSt1, closestSt2, dMinDistance1 + dMinDistance2};
    }

    std::shared_ptr<gIGrid<uint8_t>> gLayerUnderground;
private:

    std::map<uint16_t, std::list<uint32_t>> sTimeArriving; //Ordered List by Time;
    std::vector<gPosStation> sTotalStations;

    std::vector<std::pair<uint32_t, uint16_t>> gVectorTimeStation;
    uint32_t tActualStation;
    std::pair<int, int> pActualP;
};

#endif //CITYOFWEIRDFISHES_GMAINUNDERGROUND_H

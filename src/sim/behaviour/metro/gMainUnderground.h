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

class gMainUnderground {
public:

    gMainUnderground(uint32_t lSize){

    }

    struct gPosStation {
        std::pair<int, int> sPos;
        struct gRouteStation {
            gPosStation *gNextStation;
            std::vector<std::pair<int, int>> gNexRoute;
        };
        std::vector<gRouteStation> gVectNext;
    };

    void setPointsTransit(const std::vector<gPosStation> &totalStations) {
        sTotalStations = totalStations;
        uint32_t sStartTimeLoop = 10;
        uint32_t prepTime = 0;
        for (int j = 0; j < totalStations.size(); j++) {
            sTimeArriving[j] = {};
        }

        for (int i = 0; i < 10; i++) { //Nombre de voltes
            for (int j = 0; j < totalStations.size(); j++) {
                sTimeArriving[j].push_back(sStartTimeLoop + prepTime);
                gVectorTimeStation.push_back({j, sStartTimeLoop + prepTime});
                prepTime = totalStations[j].gVectNext.size() * 2;
            }
            sStartTimeLoop += prepTime;
            prepTime = 0;
        }
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
        return  sTotalStations[gVectorTimeStation[tActualStation].second].sPos;

    }

    uint32_t getClosestTimeForStationArriving(const uint16_t nStation, const uint32_t tTime) {
        for (const uint32_t tArr: sTimeArriving[nStation])
            if (tArr > tTime)
                return tArr;
        return 0;
    }

    std::shared_ptr<gIGrid<uint8_t>> gLayerUnderground;
private:

    std::map<uint16_t, std::list<uint32_t>> sTimeArriving; //Ordered List by Time;
    std::vector<gPosStation> sTotalStations;

    std::vector<std::pair<uint32_t, uint16_t>> gVectorTimeStation;
    uint32_t tActualStation;
};

#endif //CITYOFWEIRDFISHES_GMAINUNDERGROUND_H

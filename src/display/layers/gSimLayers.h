//
// Created by Laminar on 29/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GSIMLAYERS_H
#define CITYOFWEIRDFISHES_GSIMLAYERS_H

#include <utility>

#include "gILayer.h"

#include "implementation/gLayerCity.h"
#include "implementation/gLayerAirPollution.h"


enum gSimLayersTypes {
    G_CITY, G_AIRPOLLUTION, G_UNDERGROUND
};

class gSimLayers {
public:
    explicit gSimLayers(const std::shared_ptr<gLayerAirPollution>& gLAP, const std::shared_ptr<gLayerCity>& gLC,const std::pair<std::pair<int,int>,std::pair<int,int>>& rangeUse)
    : gSL_AP(gLAP), gSL_C(gLC), gRangeUse(rangeUse) {
        gSLActual = gSL_C;
    }

    void switchActual(gSimLayersTypes toChange) {
        switch (toChange) {
            case G_CITY:
                gSLActual = gSL_C;
                return;
            case G_AIRPOLLUTION:
                gSLActual = gSL_AP;
                return;
            case G_UNDERGROUND:
                return;
        }
    }
    std::pair<std::pair<int,int>,std::pair<int,int>>  gRangeUse;
    std::shared_ptr<gILayer> gSLActual;
private:

    std::shared_ptr<gLayerAirPollution> gSL_AP;
    std::shared_ptr<gLayerCity> gSL_C;
};

#endif //CITYOFWEIRDFISHES_GSIMLAYERS_H

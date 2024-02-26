//
// Created by Laminar on 29/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GSIMLAYERS_H
#define CITYOFWEIRDFISHES_GSIMLAYERS_H

#include <utility>
#include <SFML/Window/Event.hpp>

#include "gILayer.h"

#include "implementation/gLayerCity.h"
#include "implementation/gLayerAirPollution.h"
#include "implementation/gLayerTransit.h"
#include "../rRemoteUpdateGrid.h"


enum gSimLayersTypes {
    G_CITY, G_AIRPOLLUTION, G_UNDERGROUND, G_TRANSIT
};

class gSimLayers {
public:
    explicit gSimLayers(
            const std::shared_ptr<gLayerAirPollution> &gLAP,
            const std::shared_ptr<gLayerCity> &gLC,
            const std::shared_ptr<gLayerTransit> &gT,
            const std::pair<std::pair<int, int>, std::pair<int, int>> &rangeUse)
            : gSL_AP(gLAP), gSL_C(gLC), gSL_T(gT), gRangeUse(rangeUse) {
        gSLActual = gSL_C;
    }

    void switchActual(gSimLayersTypes toChange) {
        rRemoteUpdateGrid::setHasToChange(true);
        switch (toChange) {
            case G_CITY:
                gSLActual = gSL_C;
                return;
            case G_AIRPOLLUTION:
                gSLActual = gSL_AP;
                return;
            case G_TRANSIT:
                gSLActual = gSL_T;
            case G_UNDERGROUND:
                return;
        }
    }

    void pressedCell(std::pair<int, int> cPressed) {
        //std::cout << cPressed.first << ":" << cPressed.second << std::endl;
    }

    std::pair<std::pair<int, int>, std::pair<int, int>> gRangeUse;
    std::shared_ptr<gILayer> gSLActual;
private:

    std::shared_ptr<gLayerAirPollution> gSL_AP;
    std::shared_ptr<gLayerCity> gSL_C;
    std::shared_ptr<gLayerTransit> gSL_T;
};

#endif //CITYOFWEIRDFISHES_GSIMLAYERS_H

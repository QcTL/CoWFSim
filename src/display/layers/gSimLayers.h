//
// Created by Laminar on 29/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GSIMLAYERS_H
#define CITYOFWEIRDFISHES_GSIMLAYERS_H

#include <utility>

#include "gILayer.h"
#include "implementation/gLayerAirPollution.h"


enum gSimLayersTypes {
    G_CITY, G_AIRPOLLUTION, G_UNDERGROUND
};

class gSimLayers {
public:
    explicit gSimLayers(std::shared_ptr<gLayerAirPollution> gLAP) : gSL_AP(std::move(gLAP)) {
        gSLActual = gSL_AP;
    }

    void switchActual(gSimLayersTypes toChange) {
        switch (toChange) {
            case G_CITY:
                //TODO
                return;
            case G_AIRPOLLUTION:
                gSLActual = gSL_AP;
                return;
            case G_UNDERGROUND:
                return;
        }
    }

    std::shared_ptr<gILayer> gSLActual;
private:

    std::shared_ptr<gLayerAirPollution> gSL_AP;
};

#endif //CITYOFWEIRDFISHES_GSIMLAYERS_H

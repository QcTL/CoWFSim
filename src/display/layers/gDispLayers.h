//
// Created by Laminar on 29/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GDISPLAYERS_H
#define CITYOFWEIRDFISHES_GDISPLAYERS_H

#include <utility>
#include <SFML/Window/Event.hpp>

#include "gILayer.h"

#include "implementation/gLayerCity.h"
#include "implementation/gLayerAirPollution.h"
#include "implementation/gLayerTransit.h"
#include "../rRemoteUpdateGrid.h"
#include "../transformations/gTransSimToDisp.h"

enum gSimLayersTypes {
    G_CITY, G_AIRPOLLUTION, G_UNDERGROUND, G_TRANSIT
};

class gDispLayers {
public:
    explicit gDispLayers(const std::shared_ptr<gIGrid<uint8_t>> &gSimAP, const std::shared_ptr<gIGrid<uint32_t>> &gSimC,
                         const std::shared_ptr<gIGrid<uint8_t>> &gSimT)
            : gSL_AP(gTransSimToDisp::gToLayerAirPollution(gSimAP)),
              gSL_C(gTransSimToDisp::gToLayerCity(gSimC)),
              gSL_T(gTransSimToDisp::gToLayerTransit(gSimT, gSimC)),
              gRangeUse(gSimAP->rangeUse()) {
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

    void pressedCell(std::pair<int, int> cPressed) {}

    std::pair<std::pair<int, int>, std::pair<int, int>> gRangeUse;
    std::shared_ptr<gILayer> gSLActual;
private:

    std::shared_ptr<gLayerAirPollution> gSL_AP;
    std::shared_ptr<gLayerCity> gSL_C;
    std::shared_ptr<gLayerTransit> gSL_T;
};

#endif //CITYOFWEIRDFISHES_GDISPLAYERS_H

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
#include "implementation/gLayerUnderground.h"

enum gSimLayersTypes {
    G_CITY, G_AIRPOLLUTION, G_UNDERGROUND, G_TRANSIT
};

/**
 * @class gDispLayers
 * @brief This class stores the information of the matrix of all the available display layers.
 */
class gDispLayers {
public:
    explicit gDispLayers(const std::shared_ptr<gIGrid<uint8_t>> &gSimAP, const std::shared_ptr<gIGrid<uint32_t>> &gSimC,
                         const std::shared_ptr<gIGrid<uint8_t>> &gSimT, const std::shared_ptr<gIGrid<uint8_t>> &gSimU)
            : gSL_AP(gTransSimToDisp::gToLayerAirPollution(gSimAP)),
              gSL_C(gTransSimToDisp::gToLayerCity(gSimC)),
              gSL_T(gTransSimToDisp::gToLayerTransit(gSimT, gSimC)),
              gSL_U(gTransSimToDisp::gToLayerUnderground(gSimU)),
              gRangeUse(gSimAP->rangeUse()) {
        gSLActual = gSL_C;
    }

    /**
     * @fn void switchActual
     * @brief Changes the actual display layer to the one given as a parameter in a form of an enumerator
     * @param toChange The enumerator with the new layer you want to display on the screen
     */
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
                return;
            case G_UNDERGROUND:
                gSLActual = gSL_U;
                return;
        }
    }

    /**
     * @fn  void pressedCell
     * @brief The function called when a cell is pressed in the graphical interface
     * @param cPressed The pressed cell position with positive pair of coordinates
     */
    void pressedCell(std::pair<int, int> cPressed) {}

    std::pair<std::pair<int, int>, std::pair<int, int>> gRangeUse;
    std::shared_ptr<gILayer> gSLActual;
private:

    std::shared_ptr<gLayerAirPollution> gSL_AP;
    std::shared_ptr<gLayerCity> gSL_C;
    std::shared_ptr<gLayerTransit> gSL_T;
    std::shared_ptr<gLayerUnderground> gSL_U;
};

#endif //CITYOFWEIRDFISHES_GDISPLAYERS_H

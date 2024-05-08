//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GTRANSSIMTODISP_H
#define CITYOFWEIRDFISHES_GTRANSSIMTODISP_H

#include <memory>
#include "../../sim/structure/grids/gIGrid.h"
#include "../layers/implementation/gLayerAirPollution.h"
#include "../layers/implementation/gLayerCity.h"
#include "../layers/implementation/gLayerTransit.h"
#include "../layers/implementation/gLayerUnderground.h"
/**
 * @class gTransSimToDisp
 * @brief This class is responsible to hold the transformations from the basic grid to the class that represents a layer of the simulation
 */
class gTransSimToDisp {
public:

    /**
     * @fn  std::shared_ptr<gLayerAirPollution> gToLayerAirPollution
     * @param gSim The grid you want this air pollution layer to simulate simulation to follow
     * @return A Shared pointer to the newly created class representing the layer
     */
    static std::shared_ptr<gLayerAirPollution> gToLayerAirPollution(const std::shared_ptr<gIGrid<uint8_t>> &gSim) {
        return std::make_shared<gLayerAirPollution>(gLayerAirPollution(gSim));
    }

    /**
     * @fn  std::shared_ptr<gLayerAirPollution> gToLayerAirPollution
     * @param gSim The grid you want this city layer to simulate simulation to follow
     * @return A Shared pointer to the newly created class representing the layer
     */
    static std::shared_ptr<gLayerCity> gToLayerCity(const std::shared_ptr<gIGrid<uint32_t>> &gSim) {
        return std::make_shared<gLayerCity>(gLayerCity(gSim));
    }

    /**
     * @fn  std::shared_ptr<gLayerAirPollution> gToLayerAirPollution
     * @param gCitySim The grid you want this transit layer to simulate simulation to follow
     * @return A Shared pointer to the newly created class representing the layer
     */
    static std::shared_ptr<gLayerTransit>
    gToLayerTransit(const std::shared_ptr<gIGrid<uint8_t>> &gSim, const std::shared_ptr<gIGrid<uint32_t>> &gCitySim) {
        return std::make_shared<gLayerTransit>(gLayerTransit(gSim, gCitySim));
    }


    /**
     * @fn  std::shared_ptr<gLayerAirPollution> gToLayerAirPollution
     * @param gSim The grid you want this underground layer to simulate simulation to follow
     * @return A Shared pointer to the newly created class representing the layer
     */
    static std::shared_ptr<gLayerUnderground> gToLayerUnderground(const std::shared_ptr<gIGrid<uint8_t>> &gSim) {
        return std::make_shared<gLayerUnderground>(gLayerUnderground(gSim));
    }
};

#endif //CITYOFWEIRDFISHES_GTRANSSIMTODISP_H

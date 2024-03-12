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


struct cellInf;

class gTransSimToDisp {
public:
    static std::shared_ptr<gLayerAirPollution> gToLayerAirPollution(const std::shared_ptr<gIGrid<uint8_t>> &gSim) {
        return std::make_shared<gLayerAirPollution>(gLayerAirPollution(gSim));
    }

    static std::shared_ptr<gLayerCity> gToLayerCity(const std::shared_ptr<gIGrid<uint32_t>> &gSim) {
        return std::make_shared<gLayerCity>(gLayerCity(gSim));
    }

    static std::shared_ptr<gLayerTransit> gToLayerTransit(const std::shared_ptr<gIGrid<uint8_t>> &gSim) {
        return std::make_shared<gLayerTransit>(gLayerTransit(gSim));
    }
};

#endif //CITYOFWEIRDFISHES_GTRANSSIMTODISP_H

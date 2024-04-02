//
// Created by ganymede on 4/1/24.
//

#ifndef CITYOFWEIRDFISHES_GROUPLAND_H
#define CITYOFWEIRDFISHES_GROUPLAND_H

#include "sgAirPollutionMain.h"
#include "sgTerrain.h"

class groupLand {
public:
    explicit groupLand(uint32_t inGridSize) {
        gL_gAirPollution = std::make_shared<sgAirPollutionMain>(inGridSize);
        gL_gTerrain = std::make_shared<sgTerrain>(inGridSize);
    }

    void completedSetupStage() {
        gL_gTerrain->loadUpRender();
        gL_gTerrain->setupLists();
    }

    void tickReduced() {
        gL_gAirPollution->tickReduced(gL_gTerrain->gTG_TypeGen);
    }

    std::list<sgTerrain::sgT_emptySlot> getListPresentCompanies(){return gL_gTerrain->getListPresentCompanies();}

    std::shared_ptr<sgAirPollutionMain> gL_gAirPollution;
    std::shared_ptr<sgTerrain> gL_gTerrain;
};

#endif //CITYOFWEIRDFISHES_GROUPLAND_H

//
// Created by ganymede on 4/1/24.
//

#ifndef CITYOFWEIRDFISHES_GROUPLAND_H
#define CITYOFWEIRDFISHES_GROUPLAND_H

#include "sgAirPollutionMain.h"
#include "sgTerrain.h"
#include "sgUndergroundMain.h"

class groupLand {
public:
    explicit groupLand(uint32_t inGridSize) {
        gL_gAirPollution = std::make_shared<sgAirPollutionMain>(inGridSize);
        gL_gTerrain = std::make_shared<sgTerrain>(inGridSize);
        gL_gUnderground = std::make_shared<sgUndergroundMain>(inGridSize);
    }

    void completedSetupStage() {
        gL_gTerrain->loadUpRender();
        gL_gTerrain->setupLists();
    }

    void tick(uint32_t inCTime) {
        gL_gUnderground->tick(inCTime);
    }

    void tickReduced() {
        gL_gAirPollution->tickReduced(gL_gTerrain->gTG_TypeGen);
    }

    std::list<sgTerrain::sgT_CellSlot> getListPresentCompanies() { return gL_gTerrain->getListPresentCompanies(); }

    std::list<sgTerrain::sgT_CellSlot> getListEmptyCompanies() { return gL_gTerrain->getListEmptyCompanies(); }

    std::shared_ptr<sgAirPollutionMain> gL_gAirPollution;
    std::shared_ptr<sgTerrain> gL_gTerrain;
    std::shared_ptr<sgUndergroundMain> gL_gUnderground;

};

#endif //CITYOFWEIRDFISHES_GROUPLAND_H

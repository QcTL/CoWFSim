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

    /**
     * @fn void completedSetupStage
     * @brief This function should be called when the modification of the grid like sLayerType and sLayerCell has been performed
     */
    void completedSetupStage() {
        gL_gTerrain->loadUpRender();
        gL_gTerrain->setupLists();
    }

    /**
     * @fn void tick
     * @brief Updates the elements in the class that need changing every tick of the simulation
     * @param inCTime The integer containing the compressed time
     */
    void tick(uint32_t inCTime) {
        gL_gUnderground->tick(inCTime);
    }
    /**
     * @fn void tickReduced
     * @brief Updates the elements in the class that need changing every 5 tick of the simulation
     */
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

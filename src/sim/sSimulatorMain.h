//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SSIMULATORMAIN_H
#define CITYOFWEIRDFISHES_SSIMULATORMAIN_H

#include <cstdint>
#include <memory>
#include <vector>
#include "roads/node/rNode.h"
#include "roads/rNodeFromGrid.h"
#include "structure/grids/gBasicGrid.h"
#include "structure/grids/gIGrid.h"
#include "../display/rPileMenus.h"
#include "behaviour/company/sCompanyMain.h"
#include "groups/groupLand/sgUndergroundMain.h"
#include "roads/sgRoadsMain.h"
#include "behaviour/clock/sClockMain.h"
#include "groups/groupLand/groupLand.h"
#include "groups/groupEconomy/groupEconomy.h"
#include "roads/sgAStarRoad.h"

std::shared_ptr<sEventManager> sEventManager::sEM_instance = nullptr;
std::shared_ptr<eyeCatcherActive> eyeCatcherActive::instance = nullptr;
std::vector<std::string> objCompany::vSyllablesP = {"am", "ca", "mi", "o", "ul", "er", "es", "pin", "tu", "ra", "ta", "la", "dro",
                                                    "me", "dia", "mart", "sen", "ti", "tran", "qui", "li", "tat", "pen",
                                                    "sa", "ment", "u", "su", "al", "ar", "ma", "ri", "ja", "po", "nes"};
std::random_device objCompany::oC_rd;
std::mt19937 objCompany::oC_gen(objCompany::oC_rd());
std::uniform_int_distribution<> objCompany::numStringsDist(2, 3);
    //TODO :(

/**
 * @class sSimulatorMain
 * @brief This class is the main class of the simulation that host all the necessary components to run the simulation
 */
class sSimulatorMain {
public:
    explicit sSimulatorMain(int lSize) {
        sSM_GlobalTracker = stGlobalTrackerAttr::getInstance();

        gLayerNextRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(lSize, lSize, nullptr));
        sSM_groupEconomy = std::make_shared<groupEconomy>();
        sSM_groupLand = std::make_shared<groupLand>(lSize);

        sSM_gMainRoads = std::make_shared<sgRoadsMain>(lSize, sSM_groupLand);
        sSM_gAStarRoads = std::make_shared<sgAStarRoad>(sSM_gMainRoads);

        sMCivil = std::make_shared<sCivilMain>(sSM_gMainRoads, sSM_groupLand, sSM_groupEconomy);
        sSM_sCompany = std::make_shared<sCompanyMain>(lSize, sMCivil,
                                                      sSM_groupLand, sSM_groupEconomy);
        sMClock = std::make_shared<sClockMain>();
    }

    //TERRAIN BASIC;
    std::shared_ptr<groupLand> sSM_groupLand;
    std::shared_ptr<groupEconomy> sSM_groupEconomy;

    //ROADS
    std::shared_ptr<sgRoadsMain> sSM_gMainRoads;
    std::shared_ptr<sgAStarRoad> sSM_gAStarRoads;
    std::shared_ptr<gIGrid<rNode *>> gLayerNextRoad;

    std::shared_ptr<rPileMenus> rInteraction;
    std::shared_ptr<sClockMain> sMClock;

    //MEMORY:
    std::shared_ptr<sCivilMain> sMCivil;
    std::shared_ptr<sCompanyMain> sSM_sCompany;

    //TRACKERS:
    std::shared_ptr<stGlobalTrackerAttr> sSM_GlobalTracker;


    /**
     * @fn void tick
     * @brief Updates the elements in the class that need changing every tick of the simulation
     */
    void tick() {
        if (sMClock->isReducedTick()) {
            allTicksReduced(sMClock->getReduced(), sMClock->getDate());
            rInteraction->gClock->setClock(sMClock->getClock());
        }
        eyeTotalTick::pTick();
        sMClock->tick();
        sSM_groupLand->tick(sMClock->getComplete());
        sSM_gMainRoads->tick();
        sSM_gAStarRoads->tick();
    }

    /**
     * @fn void completedSetupStage
     * @brief This function should be called when all the layers have the starting values for the simulation set correctly
     */
    void completedSetupStage() {
        sSM_groupLand->completedSetupStage();
        sSM_gMainRoads->completedStartGrid();
        sSM_groupEconomy->setEmptySlots(sSM_groupLand->getListEmptyCompanies());


        //sSM_sCompany->addNewCompany(sCompanyMain::sCM_strStyleCompany::SCM_strFactory);
        sSM_sCompany->completedStartCompanies(sSM_groupLand->getListPresentCompanies());
    }

private:
    /**
     * @fn void allTicksReduced
     * @brief Calls all the function of the assigned classes that has a function that has to called each 5 ticks
     * @param inRTime The reduced time of the actual simulation < 288
     * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
     */
    void allTicksReduced(const uint32_t inRTime, const uint32_t inTDate) {
        sSM_gMainRoads->tickReduced(inRTime, inTDate);
        sSM_sCompany->tickReduced(inRTime, inTDate);
        sSM_groupLand->tickReduced();
        sSM_groupEconomy->tickReduced(inRTime, inTDate);
    }
};

#endif //CITYOFWEIRDFISHES_SSIMULATORMAIN_H

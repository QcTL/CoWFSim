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
#include "behaviour/metro/sgUndergroundMain.h"
#include "roads/sgRoadsMain.h"
#include "behaviour/clock/sClockMain.h"
#include "groups/groupLand/groupLand.h"
#include "groups/groupEconomy/groupEconomy.h"

std::shared_ptr<sEventManager> sEventManager::sEM_instance = nullptr;
std::shared_ptr<eyeCatcherActive> eyeCatcherActive::instance = nullptr;
std::vector<std::string> objCompany::vSyllablesP = {"am", "ca", "mi", "o", "ul", "er", "es", "pin", "tu", "ra", "ta", "la", "dro",
                                                    "me", "dia", "mart", "sen", "ti", "tran", "qui", "li", "tat", "pen",
                                                    "sa", "ment", "u", "su", "al", "ar", "ma", "ri", "ja", "po", "nes"};
std::random_device objCompany::oC_rd;
std::mt19937 objCompany::oC_gen(objCompany::oC_rd());
std::uniform_int_distribution<> objCompany::numStringsDist(2, 3);
    //TODO :(

class sSimulatorMain {

public:
    explicit sSimulatorMain(int lSize) {
        sSM_GlobalTracker = stGlobalTrackerAttr::getInstance();

        gLayerNextRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(lSize, lSize, nullptr));
        gTotalUnderground = std::make_shared<sgUndergroundMain>(lSize);
        sSM_groupEconomy = std::make_shared<groupEconomy>();
        sSM_groupLand = std::make_shared<groupLand>(lSize);

        gMainRoads = std::make_shared<sgRoadsMain>(lSize, sSM_groupLand);
        sMCivil = std::make_shared<sCivilMain>(gMainRoads, sSM_groupLand, sSM_groupEconomy, gTotalUnderground);
        sSM_sCompany = std::make_shared<sCompanyMain>(lSize, sMCivil,
                                                      sSM_groupLand, sSM_groupEconomy);
        sMClock = std::make_shared<sClockMain>();
    }

    //TERRAIN BASIC;

    std::shared_ptr<groupLand> sSM_groupLand;
    std::shared_ptr<groupEconomy> sSM_groupEconomy;

    //UNDERGROUND
    std::shared_ptr<sgUndergroundMain> gTotalUnderground;

    //ROADS
    std::shared_ptr<sgRoadsMain> gMainRoads;
    std::shared_ptr<gIGrid<rNode *>> gLayerNextRoad;

    std::shared_ptr<rPileMenus> rInteraction;
    std::shared_ptr<sClockMain> sMClock;

    //MEMORY:
    std::shared_ptr<sCivilMain> sMCivil;
    std::shared_ptr<sCompanyMain> sSM_sCompany;

    //TRACKERS:
    std::shared_ptr<stGlobalTrackerAttr> sSM_GlobalTracker;

    void tick() {
        if (sMClock->isReducedTick()) {
            allTicksReduced(sMClock->getReduced(), sMClock->getDate());
            rInteraction->gClock->setClock(sMClock->getClock());
        }
        eyeTotalTick::pTick();
        sMClock->tick();
        gTotalUnderground->tick(sMClock->getComplete());
        gMainRoads->tick();
    }

    void completedSetupStage() {
        sSM_groupLand->completedSetupStage();
        gMainRoads->completedStartGrid();
        sSM_groupEconomy->setEmptySlots(sSM_groupLand->getListEmptyCompanies());


        //sSM_sCompany->addNewCompany(sCompanyMain::sCM_strStyleCompany::SCM_strFactory);
        sSM_sCompany->completedStartCompanies(sSM_groupLand->getListPresentCompanies());
    }

private:
    void allTicksReduced(const uint32_t inRTime, const uint32_t inTDate) {
        gMainRoads->tickReduced(inRTime, inTDate);
        sSM_sCompany->tickReduced(inRTime, inTDate);
        sSM_groupLand->tickReduced();
        sSM_groupEconomy->tickReduced(inRTime, inTDate);
    }
};

#endif //CITYOFWEIRDFISHES_SSIMULATORMAIN_H

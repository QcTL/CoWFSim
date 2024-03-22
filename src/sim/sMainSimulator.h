//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SMAINSIMULATOR_H
#define CITYOFWEIRDFISHES_SMAINSIMULATOR_H

#include <cstdint>
#include <memory>
#include <vector>
#include "roads/node/rNode.h"
#include "roads/rTransRNodeToRRNode.h"
#include "roads/rNodeFromGrid.h"
#include "structure/grids/gBasicGrid.h"
#include "structure/grids/gIGrid.h"
#include "structure/obj/sCommon.h"
#include "structure/obj/sTotalCompany.h"
#include "structure/grids/transformation/gBaseToNearestRoad.h"
#include "../display/rPileMenus.h"
#include "structure/obj/sTotalRoutes.h"
#include "behaviour/company/sMCompany.h"
#include "behaviour/airPollution/gMainAirPollution.h"
#include "behaviour/metro/gMainUnderground.h"
#include "behaviour/gTerrainGrid.h"
#include "roads/sMainRoads.h"
#include "behaviour/market/sMainEvaluator.h"

class sMainSimulator {

public:
    explicit sMainSimulator(int lSize) {
        gMainTerrain = std::make_shared<gTerrainGrid>(lSize);
        gLayerCurStruct = std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(lSize, lSize, 0));
        gLayerNextRoad = std::make_shared<gBasicGrid<rNode *>>(gBasicGrid<rNode *>(lSize, lSize, nullptr));
        gTotalAirPollution = std::make_shared<gMainAirPollution>(lSize);
        gTotalUnderground = std::make_shared<gMainUnderground>(lSize);

        rMEvaluator = std::make_shared<sMainEvaluator>();
        gMainRoads = std::make_shared<sMainRoads>(lSize, gMainTerrain);
        sMCivil = std::make_shared<sMainCivil>(gMainRoads);
        sComp = std::make_shared<sMCompany>(lSize, sMCivil, gMainTerrain, gTotalAirPollution->gLayerAirPollution);

        gClock = {0, 0, true, 1, 1, 0};

    }

    //TERRAIN BASIC;
    std::shared_ptr<gTerrainGrid> gMainTerrain;

    std::shared_ptr<gIGrid<uint32_t>> gLayerCurStruct;
    std::shared_ptr<gIGrid<rNode *>> gLayerNextRoad;

    //AIR POLLUTION
    std::shared_ptr<gMainAirPollution> gTotalAirPollution;

    //UNDERGROUND
    std::shared_ptr<gMainUnderground> gTotalUnderground;

    //ROADS
    std::shared_ptr<sMainRoads> gMainRoads;

    std::shared_ptr<rPileMenus> rInteraction;

    std::shared_ptr<sMainEvaluator> rMEvaluator;


    rGUIClock::objClockValues gClock{};
    uint8_t pTickMinute = 0;

    void tick() {

        pTickMinute++;
        if (pTickMinute > 5) {
            pTickMinute = 0;
            gClock.rVMinute = (gClock.rVMinute + 5) % 60;
            if (gClock.rVMinute == 0) {
                gClock.rVHour = (gClock.rVHour + 1) % 13;
                if (gClock.rVHour == 0) {
                    gClock.rVHour++;
                    if (!gClock.rVIsAM) {
                        gClock.rVDay = (gClock.rVDay + 1) % 31;
                        if (gClock.rVDay == 0)
                            gClock.rVDay++;
                        if (gClock.rVMonth == 0) {
                            gClock.rVMonth = (gClock.rVMonth + 1) % 13;
                            if (gClock.rVMonth == 0)
                                gClock.rVMonth++;
                            gClock.rVYear++;
                        }
                    }
                    gClock.rVIsAM = !gClock.rVIsAM;
                }
            }

            uint32_t tReduced = gClock.rVMinute / 5 + gClock.rVHour * 12 + (gClock.rVIsAM ? 0 : 144);
            uint32_t tDate = packDateInfo(gClock.rVDay % 7, gClock.rVDay / 7, gClock.rVMonth, gClock.rVYear);
            allTicksReduced(tReduced, tDate);

            rInteraction->gClock->setClock(gClock);
        }
        gMainRoads->tick();
    }

    void completedSetupStage() {
        gMainRoads->completedStartGrid(gMainTerrain);
        gMainTerrain->setupLists();
    }

    void completedStartCompanies(const std::vector<std::vector<std::pair<int, int>>> &gPosCompanies) {
        sComp->completedStartCompanies(gPosCompanies);
    }

    //MEMORY:
    std::shared_ptr<sMainCivil> sMCivil;
    std::shared_ptr<sMCompany> sComp;

    std::shared_ptr<sTotalRoutes> sTCivil = std::make_shared<sTotalRoutes>();

private:

    void allTicksReduced(const uint32_t tReduced, const uint32_t tDate) {
        gMainRoads->tickReduced(tReduced, tDate);
        sComp->tickReduced(tReduced, tDate);
        gTotalAirPollution->tickReduced(gMainTerrain->gTG_TypeGen);
        gTotalUnderground->tickReduced(tReduced);

    }

    static uint32_t packDateInfo(uint8_t weekday, uint8_t weekNumber, uint8_t month, uint16_t year) {
        weekday &= 0b111;
        weekNumber &= 0b11;
        month &= 0b1111;
        uint32_t packedDate = 0;
        packedDate |= weekday;
        packedDate |= (uint32_t) (weekNumber) << 3;
        packedDate |= (uint32_t) (month) << 5;
        packedDate |= (uint32_t) (year) << 9;
        return packedDate;
    }
};

#endif //CITYOFWEIRDFISHES_SMAINSIMULATOR_H

//
// Created by Laminar on 02/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCONTAINERMAIN_H
#define CITYOFWEIRDFISHES_SCONTAINERMAIN_H

#include <chrono>
#include "../display/rGlobal.h"
#include "../display/menus/implementation/rBaseMenu.h"
#include "../sim/sSimulatorMain.h"

class sContainerMain {
public:

    explicit sContainerMain(const std::shared_ptr<sSimulatorMain> &sMS)
            : sMS(sMS) {

        gSimL = std::make_shared<gDispLayers>(sMS->sSM_groupLand->gL_gAirPollution->gLayerAirPollution,
                                              sMS->sSM_groupLand->gL_gTerrain->gTG_rLayer->gLayerCurStruct,
                                              sMS->sSM_gMainRoads->gLayerTransit,
                                              sMS->sSM_groupLand->gL_gUnderground->gLayerUnderground);
        pPM = std::make_shared<rPileMenus>(gSimL);
        //MENUS
        sMS->rInteraction = pPM;
        rBasic = std::make_shared<rBaseMenu>(rBaseMenu(pPM, sMS->sSM_groupLand, sMS->sSM_groupEconomy,
                                                       sMS->sSM_gMainRoads, sMS->sSM_sCompany->sTComp));
        pPM->addMenuTop(rBasic);

        rG = std::make_shared<rGlobal>(gSimL, pPM);
        rG->setUp();
    }

    void gameLoop() {
        const double targetLoopTimeMs = 1000.0 / 60.0; // Target loop time in milliseconds (60 fps)

        double lastLoopTimeMs = 0.0;
        double lastTickTimeMs = 0.0;

        //LOC VAR
        double currentTimeMs;
        double elapsedLoopTimeMs;
        double elapsedTickTimeMs;

        while (rG->isOpen) {
            currentTimeMs = getCurrentTimeInMilliseconds(); // Implement this function to get the current time

            // Cap rG.loop() at 60 fps
            elapsedLoopTimeMs = currentTimeMs - lastLoopTimeMs;
            if (elapsedLoopTimeMs >= targetLoopTimeMs) {
                rG->loop();
                lastLoopTimeMs = currentTimeMs;
            }

            // Run sMS->tickReduced() at a different frequency
            elapsedTickTimeMs = currentTimeMs - lastTickTimeMs;
            if (sMS->rInteraction->rInteractionGameVel != 0 &&
                elapsedTickTimeMs >= sMS->rInteraction->rInteractionGameVel) {
                sMS->tick();
                lastTickTimeMs = currentTimeMs;
            }
        }
    }

    static double getCurrentTimeInMilliseconds() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = currentTime.time_since_epoch();
        return (double) std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

private:
    std::shared_ptr<rGlobal> rG;
    std::shared_ptr<rPileMenus> pPM;
    std::shared_ptr<rBaseMenu> rBasic;

    std::shared_ptr<sSimulatorMain> sMS;
    std::shared_ptr<gDispLayers> gSimL;
};

#endif //CITYOFWEIRDFISHES_SCONTAINERMAIN_H

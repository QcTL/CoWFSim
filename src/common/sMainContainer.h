//
// Created by Laminar on 02/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SMAINCONTAINER_H
#define CITYOFWEIRDFISHES_SMAINCONTAINER_H

#include <chrono>
#include "../display/rGlobal.h"
#include "../display/menus/implementation/rBaseMenu.h"
#include "../sim/sMainSimulator.h"

class sMainContainer {
public:

    explicit sMainContainer(const std::shared_ptr<sMainSimulator> &sMS)
            : sMS(sMS) {

        gSimL = std::make_shared<gDispLayers>(sMS->gTotalAirPollution->gLayerAirPollution,
                                              sMS->gLayerCurStruct, sMS->gMainRoads->gLayerTransit,
                                              sMS->gTotalUnderground->gLayerUnderground);
        pPM = std::make_shared<rPileMenus>(gSimL);
        //MENUS
        sMS->rInteraction = pPM;
        rBasic = std::make_shared<rBaseMenu>(rBaseMenu(pPM, sMS->gMainTerrain->gTG_TypeGen,
                                                       sMS->gMainRoads->gLayerRoads,
                                                       sMS->sComp->gLayerOwnership, sMS->sComp->sTComp));
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

    std::shared_ptr<sMainSimulator> sMS;
    std::shared_ptr<gDispLayers> gSimL;
};

#endif //CITYOFWEIRDFISHES_SMAINCONTAINER_H

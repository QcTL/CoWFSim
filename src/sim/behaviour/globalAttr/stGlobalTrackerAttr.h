//
// Created by ganymede on 4/6/24.
//

#ifndef CITYOFWEIRDFISHES_STGLOBALTRACKERATTR_H
#define CITYOFWEIRDFISHES_STGLOBALTRACKERATTR_H

#include <memory>
#include "../../eyeCatcher/eyeValue.h"
#include "../../events/sEventReceiver.h"
#include "../recording/sSituationCivilTracker.h"
#include "../../events/sEventManager.h"

class stGlobalTrackerAttr : sEventReceiver {
public:
    stGlobalTrackerAttr() {
        std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();
        sEM->addReceiverToEvent(sEventManager::sEM_Events::sEM_EventNewCitizen, this);
        sEM->addReceiverToEvent(sEventManager::sEM_Events::sEM_EventLeaveCitizen, this);
        sEM->addReceiverToEvent(sEventManager::sEM_Events::sEM_EventStartRoute, this);
        sEM->addReceiverToEvent(sEventManager::sEM_Events::sEM_EventEndDay, this);
        sEM->addReceiverToEvent(sEventManager::sEM_Events::sEM_EventCompanyBoughtItemImport, this);
        sEM->addReceiverToEvent(sEventManager::sEM_Events::sEM_EventCompanySoldItemExport, this);

        sSituationCivilT = std::make_shared<sSituationCivilTracker>();
    };

    static std::shared_ptr<stGlobalTrackerAttr> getInstance() {
        if (!instance)
            instance = std::make_shared<stGlobalTrackerAttr>();
        return instance;
    }

    eyeValue<uint32_t> stGA_totalPopulation = eyeValue<uint32_t>("Global-Total Population", 0);
    eyeValue<uint32_t> stGA_totalEnergy = eyeValue<uint32_t>("Global-Total Energy", 0);
    eyeValue<uint32_t> stGA_totalImports = eyeValue<uint32_t>("Global-Total Imports", 0);
    eyeValue<uint32_t> stGA_totalExports = eyeValue<uint32_t>("Global-Total Exports", 0);
    eyeValue<double> stGA_ratioEmptyHouses = eyeValue<double>("Global-Ratio Empty House", 0);


    void er_EventNewCitizen(uint32_t inRTime, uint32_t inTDate,
                            uint64_t inUuidCitizen) override {
        stGA_totalPopulation += 1;
        sSituationCivilT->addNCivilian(1);

        stGA_totalEnergy.set(
                sSituationCivilT->getByTime(inRTime).first * 4 + sSituationCivilT->getByTime(inRTime).second * 3);
    }

    void er_EventLeaveCitizen(uint32_t inRTime, uint32_t inTDate,
                              uint64_t inUuidCitizen) override {
        stGA_totalPopulation -= 1;
        sSituationCivilT->removeNCivilian(1);

        stGA_totalEnergy.set(
                sSituationCivilT->getByTime(inRTime).first * 4 + sSituationCivilT->getByTime(inRTime).second * 3);
    }

    void er_EventStartRoute(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen,
                            const objActiveRute& inRoadTravel) override {
        if (!inRoadTravel.c_IsReversed)
            sSituationCivilT->addChangeSituation(inRTime, 0, 1);
        else
            sSituationCivilT->addChangeSituation(inRTime, 1, 0);

        std::cout<< sSituationCivilT->getByTime(inRTime).first << " - " << sSituationCivilT->getByTime(inRTime).second << std::endl;
        stGA_totalEnergy.set(
                sSituationCivilT->getByTime(inRTime).first * 4 + sSituationCivilT->getByTime(inRTime).second * 3);
    }

    void er_EventEndDay(uint32_t inTDate) override {
        sSituationCivilT->endDayReset();
    }

    void
    er_EventCompanyBoughtItemImport(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                    uint32_t inItemPrice) override {
        stGA_totalImports += inItemPrice;
    }

    void
    er_EventCompanySoldItemExport(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                  uint32_t inItemPrice) override {
        stGA_totalExports += inItemPrice;
    }

private:
    static std::shared_ptr<stGlobalTrackerAttr> instance;
    std::shared_ptr<sSituationCivilTracker> sSituationCivilT;
};

std::shared_ptr<stGlobalTrackerAttr> stGlobalTrackerAttr::instance = nullptr;


#endif //CITYOFWEIRDFISHES_STGLOBALTRACKERATTR_H

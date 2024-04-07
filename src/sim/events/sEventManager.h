//
// Created by ganymede on 4/7/24.
//

#ifndef CITYOFWEIRDFISHES_SEVENTMANAGER_H
#define CITYOFWEIRDFISHES_SEVENTMANAGER_H

#include <memory>
#include <map>
#include <list>
#include <algorithm>
#include "sEventReceiver.h"
#include "../structure/obj/elements/objCivil.h"

class sEventManager {
public:

    enum sEM_Events {
        sEM_EventNewCitizen = 1,
        sEM_EventLeaveCitizen = 2,
        sEM_EventCompanyCreation = 3,
        sEM_EventCompanyRemoval = 4,
        sEM_EventCompanyBoughtCell_PGiving = 5,
        sEM_EventCompanyBoughtCell_PReceiving = 6,
        sEM_EventCompanyRentCell_PGiving = 7,
        sEM_EventCompanyRentCell_PReceiving = 8,
        sEM_EventCompanyRentCellEnding_PGiving = 9,
        sEM_EventCompanyRentCellEnding_PReceiving = 10,
        sEM_EventCompanyBoughtItemLocal = 11,
        sEM_EventCompanyBoughtItemImport = 12,
        sEM_EventCompanySoldItemLocal = 13,
        sEM_EventCompanySoldItemExport = 14,
        sEM_EventStartRoute = 15,
        sEM_EventEndDay = 16,
        sEM_EventStartDay = 16,
    };

    sEventManager() = default;

    static std::shared_ptr<sEventManager> getInstance() {
        if (!sEM_instance)
            sEM_instance = std::make_shared<sEventManager>();
        return sEM_instance;
    }

    void addReceiverToEvent(const sEM_Events inEventSubscribed, sEventReceiver *inEventClass) {
        if (sEM_mapEventReceivers.find(inEventSubscribed) == sEM_mapEventReceivers.end())
            sEM_mapEventReceivers[inEventSubscribed] = {};
        sEM_mapEventReceivers[inEventSubscribed].push_back(inEventClass);
    }

    void callEventNewCitizen(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventNewCitizen])
            eR->er_EventNewCitizen(inRTime, inTDate, inUuidCitizen);
    }

    void callEventLeaveCitizen(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventLeaveCitizen])
            eR->er_EventLeaveCitizen(inRTime, inTDate, inUuidCitizen);
    }

    void callEventCompanyCreation(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyCreation])
            eR->er_EventCompanyCreation(inRTime, inTDate, inUuidCompany);
    }

    void callEventCompanyRemoval(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyRemoval])
            eR->er_EventCompanyRemoval(inRTime, inTDate, inUuidCompany);
    }

    void callEventCompanyBoughtCell_PGiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                            uint64_t inUuidContract) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyBoughtCell_PGiving])
            eR->er_EventCompanyBoughtCell_PGiving(inRTime, inTDate, inUuidCompany, inUuidContract);
    }

    void callEventCompanyBoughtCell_PReceiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                               uint64_t inUuidContract) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyBoughtCell_PReceiving])
            eR->er_EventCompanyBoughtCell_PReceiving(inRTime, inTDate, inUuidCompany, inUuidContract);
    }

    void callEventCompanyRentCell_PGiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                          uint64_t inUuidContract) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyRentCell_PGiving])
            eR->er_EventCompanyRentCell_PGiving(inRTime, inTDate, inUuidCompany, inUuidContract);
    }

    void callEventCompanyRentCell_PReceiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                             uint64_t inUuidContract) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyRentCell_PReceiving])
            eR->er_EventCompanyRentCell_PReceiving(inRTime, inTDate, inUuidCompany, inUuidContract);
    }

    void callEventCompanyRentCellEnding_PGiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                uint64_t inUuidContract) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyRentCellEnding_PGiving])
            eR->er_EventCompanyRentCellEnding_PGiving(inRTime, inTDate, inUuidCompany, inUuidContract);
    }

    void callEventCompanyRentCellEnding_PReceiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                   uint64_t inUuidContract) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyRentCellEnding_PReceiving])
            eR->er_EventCompanyRentCellEnding_PReceiving(inRTime, inTDate, inUuidCompany, inUuidContract);
    }

    void
    callEventCompanyBoughtItemLocal(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                    uint32_t inItemPrice) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyBoughtItemLocal])
            eR->er_EventCompanyBoughtItemLocal(inRTime, inTDate, inUuidCompany, inUuidItem, inItemPrice);
    }

    void
    callEventCompanyBoughtItemImport(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                     uint32_t inItemPrice) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanyBoughtItemImport])
            eR->er_EventCompanyBoughtItemImport(inRTime, inTDate, inUuidCompany, inUuidItem, inItemPrice);
    }

    void
    callEventCompanySoldItemLocal(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                  uint32_t inItemPrice) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanySoldItemLocal])
            eR->er_EventCompanySoldItemLocal(inRTime, inTDate, inUuidCompany, inUuidItem, inItemPrice);
    }

    void
    callEventCompanySoldItemExport(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                   uint32_t inItemPrice) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventCompanySoldItemExport])
            eR->er_EventCompanySoldItemExport(inRTime, inTDate, inUuidCompany, inUuidItem, inItemPrice);
    }

    void callEventStartRoute(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen,
                             const objCivil::objRoadTravel inRoadTravel) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventStartRoute])
            eR->er_EventStartRoute(inRTime, inTDate, inUuidCitizen, inRoadTravel);
    }

    void callEventEndDay(uint32_t inTDate) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventEndDay])
            eR->er_EventEndDay(inTDate);
    }

    void callEventStartDay(uint32_t inTDate) {
        for (const auto eR: sEM_mapEventReceivers[sEM_EventStartDay])
            eR->er_EventStartDay(inTDate);
    }

private:
    static std::shared_ptr<sEventManager> sEM_instance;
    std::map<uint16_t, std::list<sEventReceiver *>> sEM_mapEventReceivers;
};

#endif //CITYOFWEIRDFISHES_SEVENTMANAGER_H

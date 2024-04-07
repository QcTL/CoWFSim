//
// Created by ganymede on 4/7/24.
//

#ifndef CITYOFWEIRDFISHES_SEVENTRECEIVER_H
#define CITYOFWEIRDFISHES_SEVENTRECEIVER_H

#include <cstdint>
#include "../structure/obj/elements/objCivil.h"

class sEventReceiver {
public:
    virtual void er_EventNewCitizen(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen) {}

    virtual void er_EventLeaveCitizen(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen) {}

    virtual void er_EventCompanyCreation(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany) {}

    virtual void er_EventCompanyRemoval(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany) {}

    virtual void er_EventCompanyBoughtCell_PGiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                   uint64_t inUuidContract) {}

    virtual void er_EventCompanyBoughtCell_PReceiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                      uint64_t inUuidContract) {}

    virtual void er_EventCompanyRentCell_PGiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                 uint64_t inUuidContract) {}

    virtual void er_EventCompanyRentCell_PReceiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                    uint64_t inUuidContract) {}

    virtual void er_EventCompanyRentCellEnding_PGiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                       uint64_t inUuidContract) {}

    virtual void er_EventCompanyRentCellEnding_PReceiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                          uint64_t inUuidContract) {}

    virtual void
    er_EventCompanyBoughtItemLocal(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem, uint32_t inItemPrice) {}

    virtual void
    er_EventCompanyBoughtItemImport(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem, uint32_t inItemPrice) {}

    virtual void
    er_EventCompanySoldItemLocal(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem, uint32_t inItemPrice) {}

    virtual void
    er_EventCompanySoldItemExport(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem, uint32_t inItemPrice) {}

    virtual void er_EventStartRoute(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen,
                                    const objCivil::objRoadTravel inRoadTravel) {}

    virtual void er_EventEndDay(uint32_t inTDate) {}

    virtual void er_EventStartDay(uint32_t inTDate) {}
};

#endif //CITYOFWEIRDFISHES_SEVENTRECEIVER_H

//
// Created by ganymede on 4/7/24.
//

#ifndef CITYOFWEIRDFISHES_SEVENTRECEIVER_H
#define CITYOFWEIRDFISHES_SEVENTRECEIVER_H

#include <cstdint>
#include "../structure/obj/elements/objCivil.h"
#include "../structure/obj/sCommon.h"

/**
 * @class sEventReceiver
 * @brief Interface class that has to be implemented the selected functions the subclass wants to be notified for
 */
class sEventReceiver {
public:

    /**
     * @fn void er_EventNewCitizen
     * @brief Event called when a new Citizen is assigned to the city.
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCitizen The uuid of the new citizen.
     */
    virtual void er_EventNewCitizen(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen) {}

    /**
     * @fn void er_EventLeaveCitizen
     * @brief Event called when a citizen leaves the city
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCitizen The uuid of the leaving citizen.
     */
    virtual void er_EventLeaveCitizen(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen) {}

    /**
     * @fn void er_EventCompanyCreation
     * @brief Event called when a company is created
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCompany The uuid of the new company
     */
    virtual void er_EventCompanyCreation(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany) {}

    /**
     * @fn void er_EventCompanyRemoval
     * @brief Event called when a company is removed
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCompany The uuid of the removed company
     */
    virtual void er_EventCompanyRemoval(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany) {}

    /**
     * @fn void er_EventCompanyBoughtCell_PGiving
     * @brief One of the event called when a Cell is bought, this carries the information about the Seller
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCompany  The uuid of the selling cell company
     * @param inUuidContract The uuid of the contract added with this purchase
     */
    virtual void er_EventCompanyBoughtCell_PGiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                   uint64_t inUuidContract) {}

    /**
     * @fn void er_EventCompanyBoughtCell_PReceiving
     * @brief One of the event called when a Cell is bought, this carries the information about the Buyer
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCompany  The uuid of the buyer cell company
     * @param inUuidContract The uuid of the contract added with this purchase
     */
    virtual void er_EventCompanyBoughtCell_PReceiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                      uint64_t inUuidContract) {}

    /**
     * @fn void er_EventCompanyRentCell_PGiving
r    * @brief One of the events called when a Cell is rented, this carries the information about the seller
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCompany  The uuid of the company selling the rent of the cell
     * @param inUuidContract The uuid of the contract added with this rent
     */
    virtual void er_EventCompanyRentCell_PGiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                 uint64_t inUuidContract) {}

    /**
     * @fn void er_EventCompanyRentCell_PReceiving
     * @brief One of the events called when a Cell is rented, this carries the information about the buyer
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCompany  The uuid of the company buying the rent of the cell
     * @param inUuidContract The uuid of the contract added with this rent
     */
    virtual void er_EventCompanyRentCell_PReceiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                    uint64_t inUuidContract) {}

    /**
     * @fn void er_EventCompanyRentCellEnding_PGiving
     * @brief One of the events called when a Cell is leaving the renting period, this carries the information about the seller
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCompany  The uuid of the company selling the rent of the cell
     * @param inUuidContract The uuid of the contract added with this rent
     */
    virtual void er_EventCompanyRentCellEnding_PGiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                       uint64_t inUuidContract) {}

    /**
     * @fn void er_EventCompanyRentCellEnding_PReceiving
     * @brief One of the events called when a Cell is leaving the renting period, this carries the information about the buyer
      * @param inRTime The simulated Reduced time where the event was fired
      * @param inTDate The simulated Reduced date where the event was fired
      * @param inUuidCompany  The uuid of the company buying the rent of the cell
      * @param inUuidContract The uuid of the contract added with this rent
     */
    virtual void er_EventCompanyRentCellEnding_PReceiving(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany,
                                                          uint64_t inUuidContract) {}

    /**
     *  @fn void er_EventCompanyBoughtItemLocal
     * @brief Event called when a product is purchased locally, consumed from another company inventory.
      * @param inRTime The simulated Reduced time where the event was fired
      * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCompany The uuid of the company that bought the product
     * @param inUuidItem  The uuid of the item bought
     * @param inItemPrice The price of the purchased item.
     */
    virtual void
    er_EventCompanyBoughtItemLocal(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                   uint32_t inItemPrice) {}

    /**
     *  @fn void er_EventCompanyBoughtItemImport
     * @brief Event called when a product is purchased externally, not consumed from another company inventory.
      * @param inRTime The simulated Reduced time where the event was fired
      * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCompany The uuid of the company that bought the product
     * @param inUuidItem  The uuid of the item bought
     * @param inItemPrice The price of the purchased item.
     */
    virtual void
    er_EventCompanyBoughtItemImport(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                    uint32_t inItemPrice) {}

    /**
    *  @fn void er_EventCompanySoldItemLocal
    * @brief Event called when a product is sold locally, the company which was removed from.
    * @param inRTime The simulated Reduced time where the event was fired
    * @param inTDate The simulated Reduced date where the event was fired
    * @param inUuidCompany The uuid of the company that supplied the product
    * @param inUuidItem  The uuid of the item sold
    * @param inItemPrice The price of the sold item.
    */
    virtual void
    er_EventCompanySoldItemLocal(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                 uint32_t inItemPrice) {}

    /**
    * @fn void er_EventCompanySoldItemExport
    * @brief Event called when a product is sold externally, the company which the item was removed from.
    * @param inRTime The simulated Reduced time where the event was fired
    * @param inTDate The simulated Reduced date where the event was fired
    * @param inUuidCompany The uuid of the company that supplied the product
    * @param inUuidItem  The uuid of the sold bought
    * @param inItemPrice The price of the sold item.
    */
    virtual void
    er_EventCompanySoldItemExport(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCompany, uint32_t inUuidItem,
                                  uint32_t inItemPrice) {}

    /**
     * @fn void er_EventStartRoute
     * @brief Event called when a route is started
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     * @param inUuidCitizen The citizen that stared the rute
     * @param inRoadTravel The objectRute that followed
     */
    virtual void er_EventStartRoute(uint32_t inRTime, uint32_t inTDate, uint64_t inUuidCitizen,
                                    const objActiveRute &inRoadTravel) {}

    /**
     * @fn void er_EventEndDay
     * @brief This event is called at the end of each day
     * @param inTDate The simulated Reduced date where the event was fired
     */
    virtual void er_EventEndDay(uint32_t inTDate) {}

    /**
     * @fn void er_EventStartDay
     * @brief This event is called at the beginning of every day
     * @param inTDate The simulated Reduced date where the event was fired
     */
    virtual void er_EventStartDay(uint32_t inTDate) {}

    /**
     * @fn void er_EventCountHour
     * @brief This event is called at the beginning of every hour
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     */
    virtual void er_EventCountHour(uint32_t inRTime, uint32_t inTDate) {}


    /**
     * @fn void er_EventSoftLockCar
     * @brief This event is called at the beginning of every hour
     * @param inRTime The simulated Reduced time where the event was fired
     * @param inTDate The simulated Reduced date where the event was fired
     */
    virtual void
    er_EventSoftLockCar(const std::pair<uint32_t, uint32_t> inStrPos, const std::pair<uint32_t, uint32_t> inEndPos) {}
};

#endif //CITYOFWEIRDFISHES_SEVENTRECEIVER_H

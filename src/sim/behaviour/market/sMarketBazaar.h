//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SMARKETBAZAAR_H
#define CITYOFWEIRDFISHES_SMARKETBAZAAR_H

#include <utility>
#include <cstdint>
#include <list>
#include <algorithm>
#include <memory>
#include <cmath>

#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "listings/sIListing.h"
#include "listings/implementations/sLBuyCell.h"
#include "listings/implementations/sLRentCell.h"

/**
 * @class sMarketBazaar
 * @brief This class is the interaction with the Listing Offers for the Purchasing and the Renting cells
 */
class sMarketBazaar {
public:

    sMarketBazaar() = default;


    /**
     * @fn bool addListing
     * @brief Adds an offer into the listing for Buying cells in this class
     * @param sMO The offering you want to add into the listing
     * @return returns true if it has been correctly inserted, false otherwise
     */
    void addListing(const std::shared_ptr<sLBuyCell::sMOffering> &sMO) {
        sListingBuy.addListing(sMO);
    }

    /**
       * @fn bool addListing
       * @brief Adds an offer into the listing for Renting cells in this class
       * @param sMO The offering you want to add into the listing
       * @return returns true if it has been correctly inserted, false otherwise
       */
    void addListing(const std::shared_ptr<sLRentCell::sMOffering> &sMO) {
        sListingRent.addListing(sMO);
    }


    /**
     * @fn    std::shared_ptr<sLBuyCell::sMOffering> getListOfOffering
     * @brief Returns an offering that complies with the filter given as a parameter
     * @param filter A valid filter for Buying cells
     * @return A pointer to a valid offering that complies with the filter given
     */
    std::shared_ptr<sLBuyCell::sMOffering>
    getListOfOffering(const sLBuyCell::sMFilter &filter) {
        return sListingBuy.getOfferingByFilter(filter);
    }

    /**
     * @fn    std::shared_ptr<sLBuyCell::sMOffering> getListOfOffering
     * @brief Returns an offering that complies with the filter given as a parameter
     * @param filter A valid filter for Renting cells
     * @return A pointer to a valid offering that complies with the filter given
     */
    std::shared_ptr<sLRentCell::sMOffering>
    getListOfOffering(const sLRentCell::sMFilter &filter) {
        return sListingRent.getOfferingByFilter(filter);
    }


    /**
     * @fm void removeCompleteProcess
     * @brief Given the Offering of a Buying cell, that is contained in the listing, remove's it.
     * @param element A valid SmOffering that is in the listing that you are trying to remove it from
     */
    void removeCompleteProcess(const std::shared_ptr<sLBuyCell::sMOffering> &lCompleted) {
        sListingBuy.removeByOffering(lCompleted);
    }

    /**
     * @fm void removeCompleteProcess
     * @brief Given the Offering of a Renting cell, that is contained in the listing, remove's it.
     * @param element A valid SmOffering that is in the listing that you are trying to remove it from
     */
    void removeCompleteProcess(const std::shared_ptr<sLRentCell::sMOffering> &lCompleted) {
        sListingRent.removeByOffering(lCompleted);
    }

private:
    sIListingObj<sLBuyCell> sListingBuy;
    sIListingObj<sLRentCell> sListingRent;
};

#endif //CITYOFWEIRDFISHES_SMARKETBAZAAR_H

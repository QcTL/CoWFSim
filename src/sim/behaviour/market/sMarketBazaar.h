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

class sMarketBazaar {
public:

    sMarketBazaar() = default;

    void addListing(const std::shared_ptr<sLBuyCell::sMOffering> &sMO) {
        sListingBuy.addListing(sMO);
    }

    void addListing(const std::shared_ptr<sLRentCell::sMOffering> &sMO) {
        sListingRent.addListing(sMO);
    }

    std::shared_ptr<sLBuyCell::sMOffering>
    getListOfOffering(const sLBuyCell::sMFilter &filter) {
        return sListingBuy.getOfferingByFilter(filter);
    }

    std::shared_ptr<sLRentCell::sMOffering>
    getListOfOffering(const sLRentCell::sMFilter &filter) {
        return sListingRent.getOfferingByFilter(filter);
    }

    void removeCompleteProcess(const std::shared_ptr<sLBuyCell::sMOffering> &lCompleted) {
        sListingBuy.removeByIterator(lCompleted);
    }

    void removeCompleteProcess(const std::shared_ptr<sLRentCell::sMOffering> &lCompleted) {
        sListingRent.removeByIterator(lCompleted);
    }

private:
    sIListingObj<sLBuyCell> sListingBuy;
    sIListingObj<sLRentCell> sListingRent;
};

#endif //CITYOFWEIRDFISHES_SMARKETBAZAAR_H

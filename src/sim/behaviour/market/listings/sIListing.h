//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SILISTING_H
#define CITYOFWEIRDFISHES_SILISTING_H

#include <utility>
#include <cstdint>
#include <list>
#include <memory>
#include <algorithm>

/**
 * @class sIListing
 * @brief This class is an interface to implement the listing of offers and the filtering of the ones you want to consider
 */
class sIListing {
public:
    /**
     * @class sMOffering
     * @brief This class is an interface that implements the offering that the listing is showing and how to get if its equal
     */
    class sMOffering {
    public:
        virtual ~sMOffering() = default;

        virtual bool operator==(const sMOffering &other) const = 0;
    };

    /**
     * @class sMFilter
     * @brief This class is an interface that implements the filter of offerings that filter by some quality
     */
    class sMFilter {
    public:
        [[nodiscard]] virtual bool doesComply(const sMOffering &offer) const = 0;

        virtual ~sMFilter() = default;
    };
};

/**
 * @class sIListingObj
 * @tparam ListingType The type of listing, implemented of sIListing that you want the offers to operate from
 * @brief This is the class that you will use to interact with the sIListing class
 */
template<typename ListingType>
class sIListingObj {
public:

    template<typename U = ListingType, typename = std::enable_if_t<std::is_base_of_v<sIListing, U>>>

    /**
     * @fn bool addListing
     * @brief Adds an offer into the listing defined as a template in this class
     * @param offer The offering you want to add into the listing
     * @return returns true if it has been correctly inserted, false otherwise
     */
    bool addListing(std::shared_ptr<typename ListingType::sMOffering> offer) {
        auto insertPosition = std::find_if(sMListings.begin(), sMListings.end(), [&](const auto &item) {
            return item->operator==(*offer);
        });

        if (insertPosition != sMListings.end()) {
            sMListings.insert(insertPosition, offer);
        } else {
            sMListings.push_back(offer);
        }
        return true;
    }

    /**
     * @fn std::shared_ptr<typename ListingType::sMOffering> getOfferingByFilter
     * @brief Returns an offering that complies with the filter given as a parameter
     * @param filter A valid filter as the same implementation of the template class used
     * @return A pointer to a valid offering that complies with the filter given
     */
    std::shared_ptr<typename ListingType::sMOffering> getOfferingByFilter(const typename ListingType::sMFilter &filter) {
        auto itRet = std::find_if(sMListings.begin(), sMListings.end(), [&](const auto &item) {
            return filter.doesComply(*item);
        });
        return itRet == sMListings.end() ? nullptr : *itRet;
    }

    /**
     * @fm void removeByOffering
     * @brief Given the Offering that is contained in the listing, remove's it.
     * @param element A valid SmOffering that is in the listing that you are trying to remove it from
     */
    void removeByOffering(const std::shared_ptr<typename ListingType::sMOffering> &element) {
        sMListings.remove(element);
    }

    /**
     * @brief The destructor for the class sIListingObj
     */
    virtual ~sIListingObj() = default;

protected:
    std::list<std::shared_ptr<typename ListingType::sMOffering>> sMListings;
};


#endif //CITYOFWEIRDFISHES_SILISTING_H

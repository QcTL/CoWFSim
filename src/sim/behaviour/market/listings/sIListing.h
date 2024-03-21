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

class sIListing {
public:
    class sMOffering {
    public:
        virtual ~sMOffering() = default;

        virtual bool operator==(const sMOffering &other) const = 0;
    };

    class sMFilter {
    public:
        [[nodiscard]] virtual bool doesComply(const sMOffering &offer) const = 0;

        virtual ~sMFilter() = default;
    };
};


template<typename ListingType>
class sIListingObj {
public:

    template<typename U = ListingType, typename = std::enable_if_t<std::is_base_of_v<sIListing, U>>>

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

    std::shared_ptr<typename ListingType::sMOffering> getOfferingByFilter(const typename ListingType::sMFilter &filter) {
        return *std::find_if(sMListings.begin(), sMListings.end(), [&](const auto &item) {
            return filter.doesComply(*item);
        });
    }

    void removeByIterator(const std::shared_ptr<typename ListingType::sMOffering> &element) {
        sMListings.remove(element);
    }

    virtual ~sIListingObj() = default;

protected:
    std::list<std::shared_ptr<typename ListingType::sMOffering>> sMListings;
};


#endif //CITYOFWEIRDFISHES_SILISTING_H

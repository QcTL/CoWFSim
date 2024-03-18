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
        virtual bool doesComply(const sMOffering &offer) const = 0;

        virtual ~sMFilter() = default;
    };

    bool addListing(std::shared_ptr<sIListing::sMOffering> offer) {
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

    std::shared_ptr<sIListing::sMOffering>
    getOfferingByFilter(const sIListing::sMFilter &filter) {
        return *std::find_if(sMListings.begin(), sMListings.end(), [&](const auto &item) {
            return filter.doesComply(*item);
        });
    }

    void removeByIterator(const std::shared_ptr<sIListing::sMOffering> &element) {
        sMListings.remove(element);
    }


    virtual ~sIListing() = default;

protected:
    std::list<std::shared_ptr<sMOffering>> sMListings;
};


#endif //CITYOFWEIRDFISHES_SILISTING_H

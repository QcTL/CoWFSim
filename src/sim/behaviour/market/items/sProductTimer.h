//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SPRODUCTTIMER_H
#define CITYOFWEIRDFISHES_SPRODUCTTIMER_H

#include <utility>
#include <list>
#include <cstdint>
#include <algorithm>
#include <vector>

/**
 * @class sProductTimer
 * @brief Class responsible to set timers and notify when creating products
 */
class sProductTimer {
public:

    sProductTimer() {
        gListTimers = std::list<sObjTimer>();
    };

    /**
     * @fn void addTimer
     * @brief This function will create a timer for a product creation given the company that created and the date in which it will be created
     * @param gItemGen The valid uuid of a product, has to be less than the maximum index of product
     * @param gTimer A valid reduced date of the end time of the timer
     * @param gIdCompany A valid uuid of a company that has been created and not yet set to bankruptcy
     */
    void addTimer(uint32_t gItemGen, uint32_t gTimer, uint32_t gIdCompany) {
        sObjTimer element = {gTimer, gItemGen, gIdCompany};
        gListTimers.insert(std::find_if(gListTimers.begin(), gListTimers.end(), [&](const auto &item) {
            return item.sOT_Timer >= element.sOT_Timer;
        }), element);
    }

    /**
     * @fn  bool hasToChange
     * @param inTDate A valid reduced date of the actual time in the simulator
     * @return  True if an amount greater than 0 timers are set to complete in that date, false otherwise
     */
    bool hasToChange(uint32_t inTDate) {
        if (gListTimers.empty())
            return false;
        return gListTimers.front().sOT_Timer <= inTDate;
    };

    /**
     * @fn std::vector<std::pair<uint32_t, uint32_t>> checkForTime
     * @param inTDate  A valid reduced date of the actual time in the simulator
     * @return A list of the companies and the product that have been completed for a given date, and removed from the list
     * of remaining timers
     */
    std::vector<std::pair<uint32_t, uint32_t>> checkForTime(uint32_t inTDate) {
        std::vector<std::pair<uint32_t, uint32_t>> ret;
        while (!gListTimers.empty() && gListTimers.front().sOT_Timer <= inTDate) {
            ret.emplace_back(gListTimers.front().sOT_ItemGen, gListTimers.front().sOT_IdCompany);
            gListTimers.pop_front();
        }
        return ret;
    }

private:
    struct sObjTimer {
        uint32_t sOT_Timer;
        uint32_t sOT_ItemGen;
        uint32_t sOT_IdCompany;
    };

    std::list<sObjTimer> gListTimers;
};

#endif //CITYOFWEIRDFISHES_SPRODUCTTIMER_H

//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYTIMER_H
#define CITYOFWEIRDFISHES_SCOMPANYTIMER_H

#include <utility>
#include <list>
#include <cstdint>
#include <algorithm>
#include <vector>

class sCompanyTimer {
public:

    sCompanyTimer() = default;

    void addTimer(uint32_t gItemGen, uint32_t gTimer, uint32_t gIdCompany) {
        std::pair<uint32_t, std::pair<uint32_t, uint32_t>> element = {gTimer, {gItemGen, gIdCompany}};
        gListTimers.insert(std::find_if(gListTimers.begin(), gListTimers.end(), [&](const auto &item) {
            return item.first >= element.first;
        }), element);
    }

    bool hasToChange(uint32_t gTimer){
        return gListTimers.front().first < gTimer;
    };

    std::vector<std::pair<uint32_t, uint32_t>> checkForTime(uint32_t gTimer) {
        std::vector<std::pair<uint32_t, uint32_t>> ret;
        while (gListTimers.front().first < gTimer) {
            ret.push_back(gListTimers.front().second);
            gListTimers.pop_front();
        }
        return ret;
    }

private:

    std::list<std::pair<uint32_t, std::pair<uint32_t, uint32_t>>> gListTimers;
};

#endif //CITYOFWEIRDFISHES_SCOMPANYTIMER_H

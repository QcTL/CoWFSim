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

    sCompanyTimer() {
        gListTimers = std::list<sObjTimer>();
    };

    void addTimer(uint32_t gItemGen, uint32_t gTimer, uint32_t gIdCompany) {
        sObjTimer element = {gTimer, gItemGen, gIdCompany};
        gListTimers.insert(std::find_if(gListTimers.begin(), gListTimers.end(), [&](const auto &item) {
            return item.sOT_Timer >= element.sOT_Timer;
        }), element);
    }

    bool hasToChange(uint32_t gTimer) {
        if (gListTimers.empty())
            return false;
        return gListTimers.front().sOT_Timer <= gTimer;
    };

    std::vector<std::pair<uint32_t, uint32_t>> checkForTime(uint32_t gTimer) {
        std::vector<std::pair<uint32_t, uint32_t>> ret;
        while (!gListTimers.empty() && gListTimers.front().sOT_Timer <= gTimer) {
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

#endif //CITYOFWEIRDFISHES_SCOMPANYTIMER_H

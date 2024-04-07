//
// Created by Laminar on 06/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SSITUATIONCIVILTRACKER_H
#define CITYOFWEIRDFISHES_SSITUATIONCIVILTRACKER_H

#include <cstdint>
#include <vector>

class sSituationCivilTracker {
public:
    sSituationCivilTracker() :
            sSCProgress(std::vector<std::pair<uint16_t, uint16_t>>(288, {0, 0})),
            sSCStartHome(0), sSCStartJob(0), sSCAcumHome(0), sSCAcumJob(0) {}

    void addChangeSituation(uint32_t inRTime, uint16_t inNewHome, uint16_t inNewJob) {
        sSCProgress[inRTime].first += inNewHome;
        sSCProgress[inRTime].second += inNewJob;
        sSCAcumHome += inNewHome;
        sSCAcumJob += inNewJob;
    }

    std::pair<int, int> getByTime(uint32_t inRTime) {
        return {sSCStartHome - (sSCAcumJob - sSCAcumHome),
                sSCStartJob - (sSCAcumHome - sSCAcumJob)};
    }

    void endDayReset() {
        /*
        int _sumH = 0;
        int _sumJ = 0;

        for (const auto &p: sSCProgress) {
            _sumH += p.first;
            _sumJ += p.second;
        }
        sSCStartHome += _sumH - _sumJ;
        sSCStartJob += _sumJ - _sumH;
        */

        sSCAcumJob = 0;
        sSCAcumHome = 0;

        //TODO STORE;
    };

    void addNCivilian(uint16_t nUser) {
        sSCStartHome += nUser;
    }

    void removeNCivilian(uint16_t nUser) {
        sSCStartHome -= nUser;
    }

    [[nodiscard]] std::pair<uint32_t, uint32_t> getActualHomeJob(uint32_t inRTime) const {
        uint32_t rSHome = sSCStartHome;
        uint32_t rSJob = sSCStartJob;
        for (int i = 0; i < inRTime; i++) {
            rSHome += sSCProgress[i].first;
            rSJob += sSCProgress[i].second;
        }
        return {rSHome, rSJob};
    }

private:
    uint32_t sSCStartHome;
    uint32_t sSCStartJob;

    uint32_t sSCAcumHome;
    uint32_t sSCAcumJob;
    std::vector<std::pair<uint16_t, uint16_t>> sSCProgress;
};

#endif //CITYOFWEIRDFISHES_SSITUATIONCIVILTRACKER_H

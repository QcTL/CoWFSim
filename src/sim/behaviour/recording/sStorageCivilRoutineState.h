//
// Created by Laminar on 06/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SSTORAGECIVILROUTINESTATE_H
#define CITYOFWEIRDFISHES_SSTORAGECIVILROUTINESTATE_H

#include <cstdint>
#include <vector>

class sStorageCivilRoutineState {

    sStorageCivilRoutineState() :
            sSCProgress(std::vector<std::pair<uint16_t, uint16_t>>(288, {0, 0})),
            sSCStartHome(0), sSCStartJob(0) {}

    void tick(uint32_t inRTime, uint16_t inNewHome, uint16_t inNewJob) {
        sSCProgress[inRTime] = {inNewHome, inNewJob};
        if (inRTime == 288) { //END OF THE DAY SCENARIO
            int _sumH = 0;
            int _sumJ = 0;

            for (const auto &p: sSCProgress) {
                _sumH += p.first;
                _sumJ += p.second;
            }
            sSCStartHome += _sumH - _sumJ;
            sSCStartJob += _sumJ - _sumH;

            //TODO STORE;
        }
    }

    void addNCivilian(uint16_t nUser) {
        sSCStartHome += nUser;
    }

private:
    uint32_t sSCStartHome;
    uint32_t sSCStartJob;
    std::vector<std::pair<uint16_t, uint16_t>> sSCProgress;
};

#endif //CITYOFWEIRDFISHES_SSTORAGECIVILROUTINESTATE_H

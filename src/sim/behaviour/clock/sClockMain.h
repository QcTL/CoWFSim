//
// Created by ganymede on 3/23/24.
//

#ifndef CITYOFWEIRDFISHES_SCLOCKMAIN_H
#define CITYOFWEIRDFISHES_SCLOCKMAIN_H

#include <cstdint>
#include "../../events/sEventManager.h"

class sClockMain {
public:
    struct sCM_ClockValues {
        uint8_t sCM_rVHour = 0;
        uint8_t sCM_rVMinute = 0;
        uint8_t sCM_rVDay = 0;
        uint8_t sCM_rVMonth = 0;
        uint16_t sCM_rVYear = 0;

        uint32_t sCM_RTick = 0;
        uint32_t sCM_CDate = 0;
    };

    void tick() {
        sCM_pTickMinute++;
        if (sCM_pTickMinute > 5) {
            sCM_pTickMinute = 0;
            sCM_clock.sCM_rVMinute = (sCM_clock.sCM_rVMinute + 5) % 60;
            sCM_clock.sCM_RTick = getReduced();
            sCM_clock.sCM_CDate = getDate();
            if (sCM_clock.sCM_rVMinute == 0) {
                sCM_clock.sCM_rVHour = (sCM_clock.sCM_rVHour + 1) % 24;
                sCM_sEM->callEventCountHour(getReduced(), getDate());
                if (sCM_clock.sCM_rVHour == 0) {
                    sCM_sEM->callEventEndDay(getDate());
                    sCM_clock.sCM_rVDay = (sCM_clock.sCM_rVDay + 1) % 31;
                    sCM_sEM->callEventStartDay(getDate());
                    if (sCM_clock.sCM_rVDay == 0) {
                        sCM_clock.sCM_rVDay++;
                        sCM_clock.sCM_rVMonth = (sCM_clock.sCM_rVMonth + 1) % 13;
                        if (sCM_clock.sCM_rVMonth == 0) {
                            sCM_clock.sCM_rVMonth++;
                            sCM_clock.sCM_rVYear++;
                        }
                    }
                }
            }
            sCM_isReduced = true;
        } else
            sCM_isReduced = false;
    }

    [[nodiscard]] bool isReducedTick() const {
        return sCM_isReduced;
    }

    [[nodiscard]] uint32_t getReduced() const {
        return sCM_clock.sCM_rVMinute / 5 + sCM_clock.sCM_rVHour * 12;
    };

    [[nodiscard]] uint32_t getComplete() const {
        return sCM_clock.sCM_rVMinute + sCM_pTickMinute + sCM_clock.sCM_rVHour * 60;
    }

    [[nodiscard]] uint32_t getDate() const {
        return packDateInfo(sCM_clock.sCM_rVDay % 7, sCM_clock.sCM_rVDay / 7, sCM_clock.sCM_rVMonth,
                            sCM_clock.sCM_rVYear);
    };

    sCM_ClockValues getClock() { return sCM_clock; }

private:

    static uint32_t packDateInfo(uint8_t inWeekday, uint8_t inWeekNumber, uint8_t inMonth, uint16_t inYear) {
        inWeekday &= 0b111;
        inWeekNumber &= 0b11;
        inMonth &= 0b1111;
        uint32_t _packedDate = inWeekday;
        _packedDate |= (uint32_t) (inWeekNumber) << 3;
        _packedDate |= (uint32_t) (inMonth) << 5;
        _packedDate |= (uint32_t) (inYear) << 9;
        return _packedDate;
    }

    static sCM_ClockValues unpackDateInfo(uint32_t packedDate) {
        uint8_t outWeekday = packedDate & 0b111;
        uint8_t outWeekNumber = (packedDate >> 3) & 0b11;
        uint8_t outMonth = (packedDate >> 5) & 0b1111;
        uint16_t outYear = (packedDate >> 9);
        uint8_t totalDay = outWeekNumber * 7 + outWeekday;
        return {0, 0, totalDay, outMonth, outYear};
    }

    sCM_ClockValues sCM_clock{};
    uint8_t sCM_pTickMinute = 0;
    bool sCM_isReduced = true;
    std::shared_ptr<sEventManager> sCM_sEM = sEventManager::getInstance();
};

#endif //CITYOFWEIRDFISHES_SCLOCKMAIN_H

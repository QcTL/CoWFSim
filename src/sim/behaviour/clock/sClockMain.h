//
// Created by ganymede on 3/23/24.
//

#ifndef CITYOFWEIRDFISHES_SCLOCKMAIN_H
#define CITYOFWEIRDFISHES_SCLOCKMAIN_H

#include <cstdint>

class sClockMain {
public:
    struct sCM_ClockValues {
        uint8_t sCM_rVHour = 0;
        uint8_t sCM_rVMinute = 0;
        uint8_t sCM_rVDay = 0;
        uint8_t sCM_rVMonth = 0;
        uint16_t sCM_rVYear = 0;
    };

    void tick() {
        sCM_pTickMinute++;
        if (sCM_pTickMinute > 5) {
            sCM_pTickMinute = 0;
            sCM_clock.sCM_rVMinute = (sCM_clock.sCM_rVMinute + 5) % 60;
            if (sCM_clock.sCM_rVMinute == 0) {
                sCM_clock.sCM_rVHour = (sCM_clock.sCM_rVHour + 1) % 24;
                if (sCM_clock.sCM_rVHour == 0) {
                    sCM_clock.sCM_rVDay = (sCM_clock.sCM_rVDay + 1) % 31;
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

    [[nodiscard]] uint32_t getDate() const {
        return packDateInfo(sCM_clock.sCM_rVDay % 7, sCM_clock.sCM_rVDay / 7, sCM_clock.sCM_rVMonth, sCM_clock.sCM_rVYear);
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

    sCM_ClockValues sCM_clock{};
    uint8_t sCM_pTickMinute = 0;
    bool sCM_isReduced = true;
};

#endif //CITYOFWEIRDFISHES_SCLOCKMAIN_H

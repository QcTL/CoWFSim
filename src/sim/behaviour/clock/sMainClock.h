//
// Created by ganymede on 3/23/24.
//

#ifndef CITYOFWEIRDFISHES_SMAINCLOCK_H
#define CITYOFWEIRDFISHES_SMAINCLOCK_H

#include <cstdint>

class sMainClock {
public:
    struct objClockValues {
        uint8_t rVHour = 0;
        uint8_t rVMinute = 0;
        uint8_t rVDay = 0;
        uint8_t rVMonth = 0;
        uint16_t rVYear = 0;
    };

    void tick() {
        pTickMinute++;
        if (pTickMinute > 5) {
            pTickMinute = 0;
            gClock.rVMinute = (gClock.rVMinute + 5) % 60;
            if (gClock.rVMinute == 0) {
                gClock.rVHour = (gClock.rVHour + 1) % 24;
                if (gClock.rVHour == 0) {
                    gClock.rVDay = (gClock.rVDay + 1) % 31;
                    if (gClock.rVDay == 0) {
                        gClock.rVDay++;
                        gClock.rVMonth = (gClock.rVMonth + 1) % 13;
                        if (gClock.rVMonth == 0) {
                            gClock.rVMonth++;
                            gClock.rVYear++;
                        }
                    }
                }
            }
            isReduced = true;
        } else
            isReduced = false;
    }

    [[nodiscard]] bool isReducedTick() const {
        return isReduced;
    }

    [[nodiscard]] uint32_t getReduced() const {
        return gClock.rVMinute / 5 + gClock.rVHour * 12;
    };

    [[nodiscard]] uint32_t getDate() const {
        return packDateInfo(gClock.rVDay % 7, gClock.rVDay / 7, gClock.rVMonth, gClock.rVYear);
    };

    objClockValues getClock() { return gClock; }

private:

    static uint32_t packDateInfo(uint8_t weekday, uint8_t weekNumber, uint8_t month, uint16_t year) {
        weekday &= 0b111;
        weekNumber &= 0b11;
        month &= 0b1111;
        uint32_t packedDate = 0;
        packedDate |= weekday;
        packedDate |= (uint32_t) (weekNumber) << 3;
        packedDate |= (uint32_t) (month) << 5;
        packedDate |= (uint32_t) (year) << 9;
        return packedDate;
    }

    objClockValues gClock{};
    uint8_t pTickMinute = 0;
    bool isReduced = true;
};

#endif //CITYOFWEIRDFISHES_SMAINCLOCK_H

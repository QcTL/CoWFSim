//
// Created by ganymede on 3/23/24.
//

#ifndef CITYOFWEIRDFISHES_SMAINCLOCK_H
#define CITYOFWEIRDFISHES_SMAINCLOCK_H

#include <cstdint>
#include "../../../display/menus/implementation/AlwaysOD/rGUIClock.h"
#include "../../../display/rPileMenus.h"

class sMainClock {
public:
    void tick() {
        pTickMinute++;
        if (pTickMinute > 5) {
            pTickMinute = 0;
            gClock.rVMinute = (gClock.rVMinute + 5) % 60;
            if (gClock.rVMinute == 0) {
                gClock.rVHour = (gClock.rVHour + 1) % 13;
                if (gClock.rVHour == 0) {
                    gClock.rVHour++;
                    if (!gClock.rVIsAM) {
                        gClock.rVDay = (gClock.rVDay + 1) % 31;
                        if (gClock.rVDay == 0)
                            gClock.rVDay++;
                        if (gClock.rVMonth == 0) {
                            gClock.rVMonth = (gClock.rVMonth + 1) % 13;
                            if (gClock.rVMonth == 0)
                                gClock.rVMonth++;
                            gClock.rVYear++;
                        }
                    }
                    gClock.rVIsAM = !gClock.rVIsAM;
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
        return gClock.rVMinute / 5 + gClock.rVHour * 12 + (gClock.rVIsAM ? 0 : 144);
    };

    [[nodiscard]] uint32_t getDate() const {
        return packDateInfo(gClock.rVDay % 7, gClock.rVDay / 7, gClock.rVMonth, gClock.rVYear);
    };

    void updateClock(const std::shared_ptr<rPileMenus> &rPile) { rPile->gClock->setClock(gClock); }

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

    rGUIClock::objClockValues gClock{};
    uint8_t pTickMinute = 0;
    bool isReduced = false;
};

#endif //CITYOFWEIRDFISHES_SMAINCLOCK_H

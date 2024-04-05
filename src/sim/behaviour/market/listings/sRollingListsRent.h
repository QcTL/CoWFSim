//
// Created by ganymede on 4/4/24.
//

#ifndef CITYOFWEIRDFISHES_SROLLINGLISTSRENT_H
#define CITYOFWEIRDFISHES_SROLLINGLISTSRENT_H

#include <memory>
#include <list>
#include <vector>
#include <numeric>

class sRollingListsRent {
public:

    explicit sRollingListsRent(int wSize) : sRLR_wSize(wSize) {}

    void dropLastWindow() {
        if (!sRLR_winList.empty())
            sRLR_winList.pop_front();
    }

    void addElement(const uint32_t nValueTime) {
        if (sRLR_winList.size() >= sRLR_wSize)
            dropLastWindow();
        sRLR_winList.push_back(nValueTime);
    }

    [[nodiscard]] uint32_t getAverageBuyingTime() const {
        auto itP1 = sRLR_winList.begin();
        auto itP2 = sRLR_winList.begin();
        if (sRLR_winList.size() < 2)
            return (uint32_t) 250;
        std::vector<uint32_t> vDiffRented;

        itP1++;
        while (itP1 != sRLR_winList.end()) {
            if (*itP1 == *itP2) {
                vDiffRented.push_back(0);
                itP1++;
                itP2++;
                continue;
            }
            uint8_t P1outWeekday = *itP1 & 0b111;
            uint8_t P1outWeekNumber = (*itP1 >> 3) & 0b11;
            uint8_t P1outMonth = (*itP1 >> 5) & 0b1111;
            uint16_t P1outYear = (*itP1 >> 9);

            uint8_t P2outWeekday = *itP1 & 0b111;
            uint8_t P2outWeekNumber = (*itP1 >> 3) & 0b11;
            uint8_t P2outMonth = (*itP1 >> 5) & 0b1111;
            uint16_t P2outYear = (*itP1 >> 9);

            vDiffRented.push_back(computeDateDifference(P1outYear, P1outMonth, P1outWeekday * P1outWeekNumber,
                                                        P2outYear, P2outMonth, P2outWeekday * P2outWeekNumber));
            itP1++;
            itP2++;
        }

        uint32_t sum = std::accumulate(vDiffRented.begin(), vDiffRented.end(), (uint32_t) 0);
        return sum / vDiffRented.size();
    }

private:
    static uint32_t computeDateDifference(const uint16_t year1, const uint8_t month1, const uint8_t day1,
                                          const uint16_t year2, const uint8_t month2, const uint8_t day2) {
        int monthsDifference = (year2 - year1) * 12 + (month2 - month1);
        int daysDifference = monthsDifference * 21;
        if (day1 != 1)
            daysDifference -= (21 - day1 + 1);
        if (day2 != 1)
            daysDifference += day2 - 1;

        return daysDifference;
    }

    uint32_t sRLR_wSize;
    std::list<uint32_t> sRLR_winList;
};

#endif //CITYOFWEIRDFISHES_SROLLINGLISTSRENT_H

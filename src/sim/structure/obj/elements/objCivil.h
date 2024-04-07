//
// Created by Laminar on 16/03/2024.
//

#ifndef CITYOFWEIRDFISHES_OBJCIVIL_H
#define CITYOFWEIRDFISHES_OBJCIVIL_H

#include <utility>
#include <cstdint>

class objCivil {
public:

    enum typeRouteSystem {
        OC_TRS_TRAIN, OC_TRS_CAR
    };

    struct objRoadTravel {
        std::pair<int, int> c_RStart;
        std::pair<int, int> c_REnd;
        bool c_IsReversed;
        objRoadTravel(const std::pair<int, int> &cRStart, const std::pair<int, int> &cREnd, bool isReversed)
                : c_RStart(cRStart), c_REnd(cREnd), c_IsReversed(isReversed) {}
    };

    objCivil(typeRouteSystem cTrs, const std::pair<int,int> pHome, objRoadTravel cTravel, uint32_t cTBegin, uint32_t cTEnd, uint8_t cActiveDays)
            : c_TRS(cTrs),
              c_Travel(std::move(cTravel)),
              c_TBegin(cTBegin),
              c_TEnd(cTEnd),
              c_RActiveDays(cActiveDays) {}

    [[nodiscard]] bool isActiveDay(uint32_t cDate) const {
        return c_RActiveDays & (1 << cDate & 0x7);
    }

    uint8_t c_RActiveDays;
    typeRouteSystem c_TRS;
    objRoadTravel c_Travel;
    std::pair<int,int> c_pHome;
    uint32_t c_TBegin;
    uint32_t c_TEnd;
};

#endif //CITYOFWEIRDFISHES_OBJCIVIL_H

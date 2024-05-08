//
// Created by Laminar on 16/03/2024.
//

#ifndef CITYOFWEIRDFISHES_OBJCIVIL_H
#define CITYOFWEIRDFISHES_OBJCIVIL_H

#include <utility>
#include <cstdint>

/**
 * @class objCivil
 * @brief This class represents a civilian in the simulation with a work commute
 */
class objCivil {
public:

    enum typeRouteSystem {
        OC_TRS_TRAIN, OC_TRS_CAR
    };

    struct objRoadTravel {
        std::pair<int, int> c_RStart;
        std::pair<int, int> c_REnd;
    };

    objCivil(typeRouteSystem cTrs, const std::pair<int, int> pHome, const std::pair<int, int> pWork,
             objRoadTravel cTravel, uint32_t cTBegin, uint32_t cTEnd, uint8_t cActiveDays)
            : c_TRS(cTrs), c_Travel(std::move(cTravel)), c_pHome(pHome), c_pWork(pWork),
              c_TBegin(cTBegin), c_TEnd(cTEnd),
              c_RActiveDays(cActiveDays) {}

    /**
     * @fn bool isActiveDay
     * @param inCDate The reduced date where this creation happens, it has to be a valid reduced date
     * @return Returs true if the civilian works in that day, false otherwise
     */
    [[nodiscard]] bool isActiveDay(uint32_t inCDate) const {
        return c_RActiveDays & 1 << (inCDate & 0b111);
    }

    uint64_t c_uuid = 0;
    uint8_t c_RActiveDays;
    typeRouteSystem c_TRS;
    objRoadTravel c_Travel;
    std::pair<int, int> c_pHome;
    std::pair<int, int> c_pWork;
    uint32_t c_TBegin;
    uint32_t c_TEnd;
};

#endif //CITYOFWEIRDFISHES_OBJCIVIL_H

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

        objRoadTravel(const std::pair<int, int> &cRStart, const std::pair<int, int> &cREnd) : c_RStart(cRStart),
                                                                                              c_REnd(cREnd) {}
    };

    objCivil(typeRouteSystem cTrs, objRoadTravel cTravel, uint32_t cTBegin, uint32_t cTEnd)
            : c_TRS(cTrs),
              c_Travel(std::move(cTravel)),
              c_TBegin(cTBegin),
              c_TEnd(cTEnd) {}

    typeRouteSystem c_TRS;
    objRoadTravel c_Travel;
    uint32_t c_TBegin;
    uint32_t c_TEnd;
};

#endif //CITYOFWEIRDFISHES_OBJCIVIL_H
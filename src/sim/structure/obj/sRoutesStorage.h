//
// Created by Laminar on 03/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SROUTESSTORAGE_H
#define CITYOFWEIRDFISHES_SROUTESSTORAGE_H

#include <map>
#include <cstdint>
#include <vector>
#include <bits/shared_ptr.h>
#include "sCommon.h"
#include "elements/objCivil.h"

class sRoutesStorage {

public:

    sRoutesStorage() {
        mExitTimesCivil = std::vector<std::list<objCivil>>(289);
    };

    std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> addRuteCivil(const std::shared_ptr<objCivil> &objCivil) {
        vUniqueCivil.push_back(objCivil);

        mExitTimesCivil[objCivil->c_TBegin].push_back(*objCivil);
        mExitTimesCivil[objCivil->c_TEnd].push_back(*objCivil);

        return {mExitTimesCivil[objCivil->c_TBegin].rbegin().base(), mExitTimesCivil[objCivil->c_TEnd].rbegin().base()};
    }

    std::vector<objCivil::objRoadTravel> getRoutesCarByTime(uint32_t cTime, uint32_t cDate) {
        std::vector<objCivil::objRoadTravel> rRet;
        if (mExitTimesCivil[cTime].empty())
            return {};
        for (const auto &civ: mExitTimesCivil.at(cTime)) {
            if (civ.c_TRS == objCivil::typeRouteSystem::OC_TRS_CAR  && civ.isActiveDay(cDate))
                rRet.push_back(civ.c_TBegin == cTime ? civ.c_Travel : objCivil::objRoadTravel(civ.c_Travel.c_REnd,
                                                                                              civ.c_Travel.c_RStart));
        }
        return rRet;
    };

    std::vector<objCivil::objRoadTravel> getRoutesCarByTrain(uint32_t cTime, uint32_t cDate) {
        std::vector<objCivil::objRoadTravel> rRet;
        if (mExitTimesCivil[cTime].empty())
            return {};
        for (const auto &civ: mExitTimesCivil.at(cTime)) {
            if (civ.c_TRS == objCivil::typeRouteSystem::OC_TRS_TRAIN && civ.isActiveDay(cDate))
                rRet.push_back(civ.c_TBegin == cTime ? civ.c_Travel : objCivil::objRoadTravel(civ.c_Travel.c_REnd,
                                                                                              civ.c_Travel.c_RStart));
            //cRSTART i CREND es la dels trens, la que a ells els hi va be
        }
        return rRet;
    };

private:
    std::vector<std::shared_ptr<objCivil>> vUniqueCivil;
    std::vector<std::list<objCivil>> mExitTimesCivil;
};

#endif //CITYOFWEIRDFISHES_SROUTESSTORAGE_H

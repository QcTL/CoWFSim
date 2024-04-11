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

    [[nodiscard]]
    std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator>
    addRuteCivil(const std::shared_ptr<objCivil> &objCivil) {
        mExitTimesCivil[objCivil->c_TBegin].push_back(*objCivil);
        mExitTimesCivil[objCivil->c_TEnd].push_back(*objCivil);

        return {mExitTimesCivil[objCivil->c_TBegin].rbegin().base(), mExitTimesCivil[objCivil->c_TEnd].rbegin().base()};
    }

    void removeRuteCivil(const std::shared_ptr<objCivil> &inObjCivil,
                         const std::list<objCivil>::iterator &gPBegin,
                         const std::list<objCivil>::iterator &gPEnd) {
        mExitTimesCivil[inObjCivil->c_TBegin].erase(gPBegin);
        mExitTimesCivil[inObjCivil->c_TEnd].erase(gPEnd);
    }


    std::vector<objActiveRute> getRoutesByType(const objCivil::typeRouteSystem inRouteType, uint32_t cTime, uint32_t cDate) {
        std::vector<objActiveRute> rRet;
        if (mExitTimesCivil[cTime].empty())
            return {};
        for (const auto &civ: mExitTimesCivil.at(cTime))
            if (civ.c_TRS == inRouteType && civ.isActiveDay(cDate)) {
                if (civ.c_TBegin == cTime)
                    rRet.push_back(objActiveRute{civ.c_Travel.c_RStart, civ.c_Travel.c_REnd, false, civ.c_uuid});
                else
                    rRet.push_back(objActiveRute{civ.c_Travel.c_REnd, civ.c_Travel.c_RStart, true, civ.c_uuid});
            }
        return rRet;
    };

private:
    std::vector<std::list<objCivil>> mExitTimesCivil;
};

#endif //CITYOFWEIRDFISHES_SROUTESSTORAGE_H

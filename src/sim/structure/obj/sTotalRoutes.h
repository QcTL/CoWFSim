//
// Created by Laminar on 03/03/2024.
//

#ifndef CITYOFWEIRDFISHES_STOTALROUTES_H
#define CITYOFWEIRDFISHES_STOTALROUTES_H

#include <map>
#include <cstdint>
#include <vector>
#include <bits/shared_ptr.h>
#include "sCommon.h"

class sTotalRoutes {

public:

    sTotalRoutes() {
        mExitTimesCivil = std::vector<std::list<objCivil>>(288);
    };

    std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> addRuteCivil(const objCivil &oC) {
        auto p = std::make_shared<objCivil>(oC);
        vUniqueCivil.push_back(p);

        mExitTimesCivil[oC.c_TBegin].push_back(oC);
        mExitTimesCivil[oC.c_TEnd].push_back(oC);

        return {mExitTimesCivil[oC.c_TBegin].rbegin().base(), mExitTimesCivil[oC.c_TEnd].rbegin().base()};
    }

    std::vector<objRoadTravel> getEndStartPoints(uint32_t cTime) {
        std::vector<objRoadTravel> rRet;
        if (mExitTimesCivil[cTime].empty())
            return {};

        for (const auto &civ: mExitTimesCivil.at(cTime)) {
            if (civ.c_TBegin == cTime)
                rRet.push_back(civ.c_Travel);
            else
                rRet.push_back({civ.c_Travel.c_REnd, civ.c_Travel.c_RStart});
        }

        return rRet;
    };
private:
    std::vector<std::shared_ptr<objCivil>> vUniqueCivil;
    std::vector<std::list<objCivil>> mExitTimesCivil;

};

#endif //CITYOFWEIRDFISHES_STOTALROUTES_H

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

/**
 * @class sRoutesStorage
 * @brief This class is responsible to store the routes of the simulation
 */
class sRoutesStorage : public iStorage<objActiveRute, std::shared_ptr<objCivil>> {
public:
    sRoutesStorage() {
        mExitTimesCivil = std::vector<std::list<objCivil>>(289);
    };

    /**
     * @fn addRuteCivil
     * @brief Insert a route civil into the storage to be reference in the future
     * @param objCivil The objCivil you want to insert
     * @return The uuid of the new route inserted
     */
    uint32_t addElement(const std::shared_ptr<objCivil> &objCivil) override {
        mExitTimesCivil[objCivil->c_TBegin].push_front(*objCivil);
        mExitTimesCivil[objCivil->c_TEnd].push_front(*objCivil);
        sRS_uuidRoadIter.emplace_back(mExitTimesCivil[objCivil->c_TBegin].begin(),
                                      mExitTimesCivil[objCivil->c_TEnd].begin());
        sRS_uuidRoadTimes.emplace_back(objCivil->c_TBegin, objCivil->c_TBegin);
        return sRS_uuidRoadIter.size() - 1;
    }

    /**
     * @fn void removeRuteCivil
     * @brief Given the iterators of the route in its addition, use it to remove it from the storage
     * @param inObjCivil A pointer to a valid route that has been stored previously
     * @param gPBegin The first iterator given in the adding function
     * @param gPEnd The second iterator given in the adding function
     */
    bool removeElementByUuid(uint32_t inUuidElem) override {
        if(sRS_uuidRoadTimes.size() <= inUuidElem)
            return false;

        std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> _rItS = sRS_uuidRoadIter[inUuidElem];
        std::pair<uint8_t, uint8_t> _rTiS = sRS_uuidRoadTimes[inUuidElem];
        mExitTimesCivil[_rTiS.first].erase(_rItS.first);
        mExitTimesCivil[_rTiS.second].erase(_rItS.second);
        return true;
    }

    std::shared_ptr<objActiveRute> getElementByUuid(uint32_t inUuidElem) override{
        return nullptr;
    }

    /**
     * @fn std::vector<objActiveRute> getRoutesByType
     * @param inRouteType The type of the route you want
     * @param inRTime The reduced time of the actual simulation < 288
     * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
     * @return A list of all the routes with that specific type that start in the inRTime(hour) and in the inTDate(date)
     */
    std::vector<objActiveRute>
    getRoutesByType(const objCivil::typeRouteSystem inRouteType, uint32_t inRTime, uint32_t inTDate) {
        std::vector<objActiveRute> rRet;
        if (mExitTimesCivil[inRTime].empty())
            return {};
        for (const auto &civ: mExitTimesCivil.at(inRTime))
            if (civ.c_TRS == inRouteType && civ.isActiveDay(inTDate)) {
                if (civ.c_TBegin == inRTime)
                    rRet.push_back(objActiveRute{civ.c_Travel.c_RStart, civ.c_Travel.c_REnd, false, civ.c_uuid});
                else
                    rRet.push_back(objActiveRute{civ.c_Travel.c_REnd, civ.c_Travel.c_RStart, true, civ.c_uuid});
            }
        return rRet;
    };

private:
    std::vector<std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator>> sRS_uuidRoadIter;
    std::vector<std::pair<uint8_t, uint8_t>> sRS_uuidRoadTimes;
    std::vector<std::list<objCivil>> mExitTimesCivil;
};

#endif //CITYOFWEIRDFISHES_SROUTESSTORAGE_H

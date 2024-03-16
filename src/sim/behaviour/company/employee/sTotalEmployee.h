//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_STOTALEMPLOYEE_H
#define CITYOFWEIRDFISHES_STOTALEMPLOYEE_H

#include <cstdint>
#include <vector>
#include <memory>
#include "../../../structure/obj/sCommon.h"
#include "../../../structure/obj/sTotalRoutes.h"

class sTotalEmployee {
public:

    void addEmployeeToCompany(const objCompany &oC) {
        std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> r;
        std::pair<int, int> rPosHome = {3, 3}; // TODO CHOSE A DINAMIC ONE;

        /*lowestViableRoute lVRMetro = gMainUnderground.getLowestDistanceCommute(oC.c_cOwn[0], rPosHome);
        if (lVRMetro.totalDistance < someNumber)
            r = sTotalR->addRuteCivil(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_TRAIN,
                             {oC.c_cOwn[0], rPosHome},
                             oC.c_StrEndTime.first, oC.c_StrEndTime.second));
        else*/
            r = sTotalR->addRuteCivil(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_CAR,
                             {oC.c_cOwn[0], rPosHome},
                             oC.c_StrEndTime.first, oC.c_StrEndTime.second));



        if (vTotalCivil.find(oC.c_uuid) != vTotalCivil.end()) {
            vTotalCivil[oC.c_uuid].push_back(r.first);
            vTotalCivil[oC.c_uuid].push_back(r.second);
        } else
            vTotalCivil[oC.c_uuid] = {r.first, r.second};
    }

    std::vector<objRoadTravel> tick(uint32_t tTime){
        auto newRoutes = sTotalR->getRoutesCarByTime(tTime);
    }

private:
    std::shared_ptr<sTotalRoutes> sTotalR;
    std::map<uint32_t, std::list<std::list<objCivil>::iterator>> vTotalCivil;
    //Given company id Returns a list of iterators in the sTotalRoutes;
};

#endif //CITYOFWEIRDFISHES_STOTALEMPLOYEE_H

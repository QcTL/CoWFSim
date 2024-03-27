//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SMAINCIVIL_H
#define CITYOFWEIRDFISHES_SMAINCIVIL_H

#include <cstdint>
#include <vector>
#include <memory>
#include "../../../structure/obj/sCommon.h"
#include "../../../roads/sMainRoads.h"
#include "../../metro/gMainUnderground.h"

class sMainCivil {
public:

    explicit sMainCivil(const std::shared_ptr<sMainRoads> &sMainRoads,
                        const std::shared_ptr<gMainUnderground> &sMainUnderground)
            : sMC_sMainRoads(sMainRoads), sMC_sMainUnderground(sMainUnderground) {};

    void addEmployeeToCompany(objCompany &oC) {
        std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> r;
        std::pair<int, int> rPosCivHome = sMC_sMainRoads->getRandomCivilStartRoad();

        gMainUnderground::lowestViableRoute lVRMetro = sMC_sMainUnderground->getLowestDistanceCommute(
                oC.c_cActiveLocations.front(), rPosCivHome);

        if (lVRMetro.totalDistance < 1000)
            r = sMC_sMainRoads->addRuteCivil(std::make_shared<objCivil>(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_TRAIN,
                             {oC.c_cActiveLocations.front(), rPosCivHome},
                             oC.c_activeDates.c_StrEndTime.first, oC.c_activeDates.c_StrEndTime.second,
                             oC.c_activeDates.cAD_jobWeek)));
        else {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(-5, 5); //TODO based on Seed;
            std::shared_ptr<objCivil> oCivil = std::make_shared<objCivil>(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_CAR,
                             {sMC_sMainRoads->getClosestRoadToBuilding(oC.c_cActiveLocations.front()), rPosCivHome},
                             oC.c_activeDates.c_StrEndTime.first + distrib(gen),
                             oC.c_activeDates.c_StrEndTime.second + distrib(gen),
                             oC.c_activeDates.cAD_jobWeek));
            r = sMC_sMainRoads->addRuteCivil(oCivil);
        }

        if (vTotalCivil.find(oC.c_uuid) != vTotalCivil.end()) {
            vTotalCivil[oC.c_uuid].push_back(r.first);
            vTotalCivil[oC.c_uuid].push_back(r.second);
        } else
            vTotalCivil[oC.c_uuid] = {r.first, r.second};

        oC.c_nEmployee += 1;
    }

    std::vector<objCivil::objRoadTravel> tick(uint32_t tTime) {
        return {};
    }

private:
    std::shared_ptr<sMainRoads> sMC_sMainRoads;
    std::shared_ptr<gMainUnderground> sMC_sMainUnderground;
    std::map<uint32_t, std::list<std::list<objCivil>::iterator>> vTotalCivil;
    //Given company id Returns a list of iterators in the sTotalRoutes;
};

#endif //CITYOFWEIRDFISHES_SMAINCIVIL_H

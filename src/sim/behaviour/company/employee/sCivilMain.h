//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCIVILMAIN_H
#define CITYOFWEIRDFISHES_SCIVILMAIN_H

#include <cstdint>
#include <vector>
#include <memory>
#include "../../../structure/obj/sCommon.h"
#include "../../../roads/sgRoadsMain.h"
#include "../../metro/sgUndergroundMain.h"

class sCivilMain {
public:

    explicit sCivilMain(const std::shared_ptr<sgRoadsMain> &inPsRoadsMain,
                        const std::shared_ptr<sgUndergroundMain> &inPSUndergroundMain)
            : sCM_sRoadsMain(inPsRoadsMain), sCM_sUndergroundMain(inPSUndergroundMain) {};

    void addEmployeeToCompany(objCompany &inObjCompany) {
        std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> r;
        std::pair<int, int> rPosCivHome = sCM_sRoadsMain->getRandomCivilStartRoad();

        sgUndergroundMain::sgUM_lowestViableRoute lVRMetro = sCM_sUndergroundMain->getLowestDistanceCommute(
                inObjCompany.c_cActiveLocations.front(), rPosCivHome);

        if (lVRMetro.totalDistance < 100)
            r = sCM_sRoadsMain->addRuteCivil(std::make_shared<objCivil>(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_TRAIN,
                             {inObjCompany.c_cActiveLocations.front(), rPosCivHome},
                             sCM_sUndergroundMain->getClosestTimeForStation(lVRMetro.closestSt1,
                                                                            inObjCompany.c_activeDates.c_StrEndTime.first),
                             sCM_sUndergroundMain->getClosestTimeForStation(lVRMetro.closestSt2,
                                                                            inObjCompany.c_activeDates.c_StrEndTime.second),
                             inObjCompany.c_activeDates.cAD_jobWeek)));
        else {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(-5, 5); //TODO based on Seed;
            std::shared_ptr<objCivil> oCivil = std::make_shared<objCivil>(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_CAR,
                             {sCM_sRoadsMain->getClosestRoadToBuilding(inObjCompany.c_cActiveLocations.front()), rPosCivHome},
                             inObjCompany.c_activeDates.c_StrEndTime.first + distrib(gen),
                             inObjCompany.c_activeDates.c_StrEndTime.second + distrib(gen),
                             inObjCompany.c_activeDates.cAD_jobWeek));
            r = sCM_sRoadsMain->addRuteCivil(oCivil);
        }

        if (vTotalCivil.find(inObjCompany.c_uuid) != vTotalCivil.end()) {
            vTotalCivil[inObjCompany.c_uuid].push_back(r.first);
            vTotalCivil[inObjCompany.c_uuid].push_back(r.second);
        } else
            vTotalCivil[inObjCompany.c_uuid] = {r.first, r.second};

        inObjCompany.c_nEmployee += 1;
    }

    std::vector<objCivil::objRoadTravel> tick(uint32_t inRTime) {
        return {};
    }

private:
    std::shared_ptr<sgRoadsMain> sCM_sRoadsMain;
    std::shared_ptr<sgUndergroundMain> sCM_sUndergroundMain;
    std::map<uint32_t, std::list<std::list<objCivil>::iterator>> vTotalCivil;
    //Given company id Returns a list of iterators in the sRoutesStorage;
};

#endif //CITYOFWEIRDFISHES_SCIVILMAIN_H

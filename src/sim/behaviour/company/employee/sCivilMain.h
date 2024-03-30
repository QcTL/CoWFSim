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
            : sCM_sRoadsMain(inPsRoadsMain), sCM_sUndergroundMain(inPSUndergroundMain) {
        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            sCM_genRand.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            sCM_genRand.seed(static_cast<unsigned int>(time(nullptr)));
    };

    void addEmployeeToCompany(objCompany &inObjCompany) {
        std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> r;
        std::pair<int, int> rPosCivHome = sCM_sRoadsMain->getNewRandomAssignedCivilHome();

        sgUndergroundMain::sgUM_lowestViableRoute lVRMetro = sCM_sUndergroundMain->getLowestDistanceCommute(
                inObjCompany.c_cActiveLocations.front(), rPosCivHome);
        std::shared_ptr<objCivil> _oCivil;
        if (lVRMetro.totalDistance < 100)
            _oCivil = std::make_shared<objCivil>(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_TRAIN,
                             {inObjCompany.c_cActiveLocations.front(), rPosCivHome},
                             sCM_sUndergroundMain->getClosestTimeForStation(lVRMetro.closestSt1,
                                                                            inObjCompany.c_activeDates.c_StrEndTime.first),
                             sCM_sUndergroundMain->getClosestTimeForStation(lVRMetro.closestSt2,
                                                                            inObjCompany.c_activeDates.c_StrEndTime.second),
                             inObjCompany.c_activeDates.cAD_jobWeek));
        else {
            std::uniform_int_distribution<> distrib(-5, 5);
            _oCivil = std::make_shared<objCivil>(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_CAR,
                             {sCM_sRoadsMain->getClosestRoadToBuilding(inObjCompany.c_cActiveLocations.front()),
                              rPosCivHome},
                             inObjCompany.c_activeDates.c_StrEndTime.first + distrib(sCM_genRand),
                             inObjCompany.c_activeDates.c_StrEndTime.second + distrib(sCM_genRand),
                             inObjCompany.c_activeDates.cAD_jobWeek));
        }
        r = sCM_sRoadsMain->addRuteCivil(_oCivil);

        if (vTotalCivil.find(inObjCompany.c_uuid) != vTotalCivil.end())
            vTotalCivil[inObjCompany.c_uuid].push_back({_oCivil, r.first, r.second});
        else
            vTotalCivil[inObjCompany.c_uuid] = {{_oCivil, r.first, r.second}};

        inObjCompany.c_nEmployee += 1;
    }

    void removeEmployeeToCompany(objCompany &inObjCompany) {
        //S'ha de obtenir de vTotalCivil i s'ha de treure de sCM_sRoadsMain;
        if (vTotalCivil[inObjCompany.c_uuid].empty())
            return;

        sCM_cCivRoute _cRoute = vTotalCivil[inObjCompany.c_uuid].front();
        vTotalCivil[inObjCompany.c_uuid].pop_front();
        sCM_sRoadsMain->removeRuteCivil(_cRoute.sCM_cCRCivil, _cRoute.sCM_cCRUrBegin, _cRoute.sCM_cCRUrEnd);
    }

    std::vector<objCivil::objRoadTravel> tick(uint32_t inRTime) {
        return {};
    }

private:
    struct sCM_cCivRoute {
        std::shared_ptr<objCivil> sCM_cCRCivil;
        std::list<objCivil>::iterator sCM_cCRUrBegin;
        std::list<objCivil>::iterator sCM_cCRUrEnd;
    };
    std::mt19937 sCM_genRand;

    std::shared_ptr<sgRoadsMain> sCM_sRoadsMain;
    std::shared_ptr<sgUndergroundMain> sCM_sUndergroundMain;
    std::map<uint32_t, std::list<sCM_cCivRoute>> vTotalCivil;
    //Given company id Returns a list of iterators in the sRoutesStorage;
};

#endif //CITYOFWEIRDFISHES_SCIVILMAIN_H

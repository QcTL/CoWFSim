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
#include "../../../groups/groupEconomy/groupEconomy.h"

class sCivilMain {
public:

    explicit sCivilMain(const std::shared_ptr<sgRoadsMain> &inPsRoadsMain,
                        const std::shared_ptr<groupLand> &inGLand, const std::shared_ptr<groupEconomy> &inGEconomy,
                        const std::shared_ptr<sgUndergroundMain> &inPSUndergroundMain)
            : sCM_sRoadsMain(inPsRoadsMain), sCM_groupLand(inGLand), sCM_groupEconomy(inGEconomy),
              sCM_sUndergroundMain(inPSUndergroundMain) {
        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            sCM_genRand.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            sCM_genRand.seed(static_cast<unsigned int>(time(nullptr)));
    };

    std::pair<int, int> getNewValidHouse() {
        std::uniform_int_distribution<int> distribution(1, 3);
        uint8_t gPrefLivingType = distribution(sCM_genRand);
        auto gPos = sCM_groupLand->gL_gTerrain->getHomeSortLessQuality(gPrefLivingType);
        return gPos;
    }

    uint32_t getPriceByHouse(const std::pair<int, int> &inPCell) {
        uint32_t gPosBasePrice = sCM_groupLand->gL_gTerrain->getQualityGivenPosHome(inPCell);
        uint8_t gPrefLivingType = sCM_groupLand->gL_gTerrain->gTG_TypeSoil->get(inPCell);
        double dUnitHouses = std::min(sCM_groupLand->gL_gTerrain->getRemainHomes(gPrefLivingType), 0.99);

        double gFactorRarity = (1 / dUnitHouses);
        uint32_t _incrFactor = 50; //Relation between the inverse of the distance of times and the incremental factor
        double gPremium =
                (1.0 / std::sqrt((double) sCM_groupEconomy->gE_sRLR->getAverageBuyingTime() + 1)) * _incrFactor;

        return (uint32_t) (gPosBasePrice + gPremium * gFactorRarity);
    }

    void setRouteToNewEmployee(std::pair<int, int> rPosCivHome, const objCompany &inObjCompany) {
        std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> r;

        //TODO remove them from the pool of actives.
        sgUndergroundMain::sgUM_lowestViableRoute lVRMetro = sCM_sUndergroundMain->getLowestDistanceCommute(
                inObjCompany.c_cActiveLocations.front(), rPosCivHome);
        std::shared_ptr<objCivil> _oCivil;
        if (lVRMetro.totalDistance < 30) {
            std::cout << "POINTO METRO" << std::endl;
            _oCivil = std::make_shared<objCivil>(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_TRAIN, rPosCivHome,
                             {inObjCompany.c_cActiveLocations.front(), rPosCivHome,
                              false}, // TODO POSAR LES ESTACIONS DE TREN
                             sCM_sUndergroundMain->getClosestTimeForStation(lVRMetro.closestSt1,
                                                                            inObjCompany.c_activeDates.c_StrEndTime.first),
                             sCM_sUndergroundMain->getClosestTimeForStation(lVRMetro.closestSt2,
                                                                            inObjCompany.c_activeDates.c_StrEndTime.second),
                             inObjCompany.c_activeDates.cAD_jobWeek));
        } else {
            std::uniform_int_distribution<> distrib(-5, 5);
            _oCivil = std::make_shared<objCivil>(
                    objCivil(objCivil::typeRouteSystem::OC_TRS_CAR, rPosCivHome,
                             {sCM_sRoadsMain->getClosestRoadToBuilding(inObjCompany.c_cActiveLocations.front()),
                              sCM_sRoadsMain->getClosestRoadToBuilding(rPosCivHome), false},
                             inObjCompany.c_activeDates.c_StrEndTime.first + distrib(sCM_genRand),
                             inObjCompany.c_activeDates.c_StrEndTime.second + distrib(sCM_genRand),
                             inObjCompany.c_activeDates.cAD_jobWeek));
        }
        r = sCM_sRoadsMain->addRuteCivil(_oCivil);

        if (vTotalCivil.find(inObjCompany.c_uuid) != vTotalCivil.end())
            vTotalCivil[inObjCompany.c_uuid].push_back({_oCivil, r.first, r.second});
        else
            vTotalCivil[inObjCompany.c_uuid] = {{_oCivil, r.first, r.second}};
    }

    std::pair<int, int> removeEmployeeToCompany(const objCompany &inObjCompany) {
        //S'ha de obtenir de vTotalCivil i s'ha de treure de sCM_sRoadsMain;
        if (vTotalCivil[inObjCompany.c_uuid].empty())
            return {};

        sCM_cCivRoute _cRoute = vTotalCivil[inObjCompany.c_uuid].front();
        vTotalCivil[inObjCompany.c_uuid].pop_front();
        sCM_sRoadsMain->removeRuteCivil(_cRoute.sCM_cCRCivil, _cRoute.sCM_cCRUrBegin, _cRoute.sCM_cCRUrEnd);

        return _cRoute.sCM_cCRCivil->c_pHome;
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
    std::shared_ptr<groupLand> sCM_groupLand;
    std::shared_ptr<groupEconomy> sCM_groupEconomy;
    std::shared_ptr<sgUndergroundMain> sCM_sUndergroundMain;
    std::map<uint32_t, std::list<sCM_cCivRoute>> vTotalCivil;
    //Given company id Returns a list of iterators in the sRoutesStorage;
};

#endif //CITYOFWEIRDFISHES_SCIVILMAIN_H

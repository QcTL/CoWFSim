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
#include "sCivilStorage.h"

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
        sCM_civilStorage = std::make_shared<sCivilStorage>();
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


    std::shared_ptr<objCivil> createCivil(std::pair<int, int> rPosCivHome, const objCompany &inObjCompany) {
        std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> r;
        std::shared_ptr<objCivil> _oCivil;

        sgUndergroundMain::sgUM_lowestViableRoute lVRMetro = sCM_sUndergroundMain->getLowestDistanceCommute(
                inObjCompany.c_cActiveLocations.front(), rPosCivHome);

        if (lVRMetro.totalDistance < 30)
            _oCivil = _getCivilMetro(rPosCivHome, inObjCompany, lVRMetro);
        else
            _oCivil = _getCivilRoad(rPosCivHome, inObjCompany);

        r = sCM_sRoadsMain->addRuteCivil(_oCivil);
        _oCivil->c_uuid = sCM_civilStorage->storeCivil(_oCivil, inObjCompany.c_uuid, r);
        return _oCivil;
    }

    std::shared_ptr<objCivil> removeEmployeeToCompany(const uint32_t inUuidCompany) {
        sCivilStorage::sCS_cCivRoute _cRoute = sCM_civilStorage->routeCivilGivenCompany(inUuidCompany);
        sCM_sRoadsMain->removeRuteCivil(sCM_civilStorage->getByUuid(_cRoute.sCS_cCRCivil), _cRoute.sCS_cCRUrBegin, _cRoute.sCS_cCRUrEnd);
        return sCM_civilStorage->getByUuid(_cRoute.sCS_cCRCivil);
    }

private:
    std::shared_ptr<objCivil> _getCivilMetro(const std::pair<int, int> &rPosCivHome, const objCompany &inObjCompany,
                                             const sgUndergroundMain::sgUM_lowestViableRoute &rRouteTrain) {
        return std::make_shared<objCivil>(
                objCivil(objCivil::typeRouteSystem::OC_TRS_TRAIN, rPosCivHome,
                         {inObjCompany.c_cActiveLocations.front(), rPosCivHome,
                          false}, // TODO POSAR LES ESTACIONS DE TREN
                         sCM_sUndergroundMain->getClosestTimeForStation(rRouteTrain.closestSt1,
                                                                        inObjCompany.c_activeDates.c_StrEndTime.first),
                         sCM_sUndergroundMain->getClosestTimeForStation(rRouteTrain.closestSt2,
                                                                        inObjCompany.c_activeDates.c_StrEndTime.second),
                         inObjCompany.c_activeDates.cAD_jobWeek));
    }

    std::shared_ptr<objCivil> _getCivilRoad(const std::pair<int, int> &rPosCivHome, const objCompany &inObjCompany) {
        int sOffsetTimeStart = 5;
        std::uniform_int_distribution<> distrib(-sOffsetTimeStart, sOffsetTimeStart);
        return std::make_shared<objCivil>(
                objCivil(objCivil::typeRouteSystem::OC_TRS_CAR, rPosCivHome,
                         {sCM_sRoadsMain->getClosestRoadToBuilding(inObjCompany.c_cActiveLocations.front()),
                          sCM_sRoadsMain->getClosestRoadToBuilding(rPosCivHome), false},
                         inObjCompany.c_activeDates.c_StrEndTime.first + distrib(sCM_genRand),
                         inObjCompany.c_activeDates.c_StrEndTime.second + distrib(sCM_genRand),
                         inObjCompany.c_activeDates.cAD_jobWeek));
    }
    std::mt19937 sCM_genRand;

    std::shared_ptr<sgRoadsMain> sCM_sRoadsMain;
    std::shared_ptr<groupLand> sCM_groupLand;
    std::shared_ptr<groupEconomy> sCM_groupEconomy;
    std::shared_ptr<sgUndergroundMain> sCM_sUndergroundMain;
    std::shared_ptr<sCivilStorage> sCM_civilStorage;
    //Given company id Returns a list of iterators in the sRoutesStorage;
};

#endif //CITYOFWEIRDFISHES_SCIVILMAIN_H

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
#include "../../../groups/groupLand/sgUndergroundMain.h"
#include "../../../groups/groupEconomy/groupEconomy.h"
#include "sCivilStorage.h"

/**
 * @class sCivilMain
 * @brief Class that controls the main aspects and storage of the civilians
 */
class sCivilMain {
public:

    explicit sCivilMain(const std::shared_ptr<sgRoadsMain> &inPsRoadsMain,
                        const std::shared_ptr<groupLand> &inGLand, const std::shared_ptr<groupEconomy> &inGEconomy)
            : sCM_sRoadsMain(inPsRoadsMain), sCM_groupLand(inGLand), sCM_groupEconomy(inGEconomy) {
        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            sCM_genRand.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            sCM_genRand.seed(static_cast<unsigned int>(time(nullptr)));
        sCM_civilStorage = std::make_shared<sCivilStorage>();
    };

    /**
     * @fn std::pair<int, int> getNewValidHouse
     * @brief This function returns a house with the attribute to be able to host one more civilian inside
     * @return A Positive pair of coordinates of a house that can host one more civilian. Sorted by Quality
     */
    std::pair<int, int> getNewValidHouse() {
        std::uniform_int_distribution<int> distribution(1, 3);
        uint8_t gPrefLivingType = distribution(sCM_genRand);
        auto gPos = sCM_groupLand->gL_gTerrain->getHomeSortLessQuality(gPrefLivingType);
        return gPos;
    }

    /**
     * @fn uint32_t getPriceByHouse
     * @brief Given a position of a valid house returns the price of acquiring it in that moment based on other attributes
     * in the simulation
     * @param inPCell A Pair of positive coordinates that represent a position in the simulation that has to contain a house
     * @return The price for that house on that given moment
     */
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

    /**
     * @fn std::shared_ptr<objCivil> createCivil
     * @brief Given the position of the hosting of the new civilian, the company in which they will work and the date of the actual simulation
     * store and act upon a new created civilian.
     * @param rPosCivHome The position of a valid home that has the enough space to be set in.
     * @param inObjCompany An existing company that the civilian will work for
     * @param inTDate The compacted current date of the simulation where it occurs
     * @return It returns a shared pointer to the newly created civilian.
     */
    std::shared_ptr<objCivil>
    createCivil(std::pair<int, int> rPosCivHome, const objCompany &inObjCompany, uint32_t inTDate) {
        uint32_t _uuidRute;
        std::shared_ptr<objCivil> _oCivil;

        sgUndergroundMain::sgUM_lowestViableRoute lVRMetro = sCM_groupLand->gL_gUnderground->getLowestDistanceCommute(
                rPosCivHome, inObjCompany.c_cActiveLocations.begin()->second.front());

        if (lVRMetro.totalDistance < 30) {
            _oCivil = _getCivilMetro(rPosCivHome, inObjCompany, lVRMetro);
            _uuidRute = sCM_sRoadsMain->addRuteCivil(_oCivil);
        } else {
            _oCivil = _getCivilRoad(rPosCivHome, inObjCompany);
            _uuidRute = sCM_sRoadsMain->addRuteCivil(_oCivil);
        }
        _oCivil->c_uuid = sCM_civilStorage->storeCivil(_oCivil, inObjCompany.c_uuid, _uuidRute);

        sCM_groupEconomy->gE_sRLR->addElement(inTDate);
        sCM_groupLand->gL_gTerrain->addCivilHomeToPos(rPosCivHome);

        sCM_cTotalCivil++;
        return _oCivil;
    }

    /**
     * @fn  std::shared_ptr<objCivil> getCivGivenCompany
     * @brief Get by the given company a pointer to a civilian that works there
     * @param inUuidCompany The uuid of the company you want to search it, has to be valid
     * @return A shared pointer to the objCivil that works in that company
     */
    std::shared_ptr<objCivil> getCivGivenCompany(const uint32_t inUuidCompany) {
        sCivilStorage::sCS_cCivRoute _cRoute = sCM_civilStorage->routeCivilGivenCompany(inUuidCompany);
        return sCM_civilStorage->getByUuid(_cRoute.sCS_cCRCivil);
    }

    /**
     * @fn void removeRouteGivenCivil
     * @brief Given a pointer to a civilian and the uuid of the company that works for, removes from the active routes that happen this
     * concrete civilian
     * @param inCivil A valid civilian that works for the company referred with inUuidCompany
     * @param inUuidCompany A valid company uuid that has as a employee the inCivil given as a parameter.
     */
    void removeRouteGivenCivil(const std::shared_ptr<objCivil> &inCivil, const uint32_t inUuidCompany) {
        sCivilStorage::sCS_cCivRoute _cRoute = sCM_civilStorage->routeCivilGivenCompany(inUuidCompany);
        sCM_cTotalCivil--;
        sCM_sRoadsMain->removeRuteCivil(_cRoute.sCS_cCRRouteUuid);
    }

    /**
     * @fn void deleteCivil
     * @brief Removes a civil of the active civilian of the city given a pointer to it and the uuid of the company that hired him
     * @param inCivil A valid civilian that works for the company referred with inUuidCompany
     * @param inUuidCompany A valid company uuid that has as a employee the inCivil given as a parameter.
     */
    void deleteCivil(const std::shared_ptr<objCivil> &inCivil, const uint32_t inUuidCompany) {
        sCM_civilStorage->removeCivilGivenCompany(inUuidCompany);
        sCM_cTotalCivil--;
    }

    /**
     * @fn uint64_t getNCivil
     * @return Returns the total of civilians that exists on the city.
     */
    [[nodiscard]]uint64_t getNCivil() const {
        return sCM_cTotalCivil;
    }

private:

    /**
     * @fn std::shared_ptr<objCivil> _getCivilMetro
     * @brief Generates the object of objCivil that has as a method of commute to work the Metro
     * @param rPosCivHome The position of the valid home for the newly created civilian, it has to have at least a free space
     * @param inObjCompany A reference to the company were the new civilian will work
     * @param rRouteTrain The Metro route that the civilian will follow in his daily commute
     * @return A pointer to the newly created civilian
     */
    std::shared_ptr<objCivil> _getCivilMetro(const std::pair<int, int> &rPosCivHome, const objCompany &inObjCompany,
                                             const sgUndergroundMain::sgUM_lowestViableRoute &rRouteTrain) {
        return std::make_shared<objCivil>(
                objCivil(objCivil::typeRouteSystem::OC_TRS_TRAIN, rPosCivHome,
                         inObjCompany.c_cActiveLocations.begin()->second.front(),
                         {sCM_groupLand->gL_gUnderground->getPosStationById(rRouteTrain.closestSt1),
                          sCM_groupLand->gL_gUnderground->getPosStationById(rRouteTrain.closestSt2)},
                         sCM_groupLand->gL_gUnderground->getClosestTimeForStation(rRouteTrain.closestSt1,
                                                                                  inObjCompany.c_activeDates.c_StrEndTime.first),
                         sCM_groupLand->gL_gUnderground->getClosestTimeForStation(rRouteTrain.closestSt2,
                                                                                  inObjCompany.c_activeDates.c_StrEndTime.second),
                         inObjCompany.c_activeDates.cAD_jobWeek));
    }


    /**
     * @fn std::shared_ptr<objCivil> _getCivilMetro
     * @brief Generates the object of objCivil that has as a method of commute to work the car.
     * @param rPosCivHome The position of the valid home for the newly created civilian, it has to have at least a free space
     * @param inObjCompany A reference to the company were the new civilian will work
     * @return A pointer to the newly created civilian
     */
    std::shared_ptr<objCivil> _getCivilRoad(const std::pair<int, int> &rPosCivHome, const objCompany &inObjCompany) {
        int sOffsetTimeStart = 5;
        std::uniform_int_distribution<> distrib(-sOffsetTimeStart, sOffsetTimeStart);
        return std::make_shared<objCivil>(
                objCivil(objCivil::typeRouteSystem::OC_TRS_CAR, rPosCivHome,
                         inObjCompany.c_cActiveLocations.begin()->second.front(),
                         {sCM_sRoadsMain->getClosestRoadToBuilding(rPosCivHome),
                          sCM_sRoadsMain->getClosestRoadToBuilding(
                                  inObjCompany.c_cActiveLocations.begin()->second.front())},
                         inObjCompany.c_activeDates.c_StrEndTime.first + distrib(sCM_genRand),
                         inObjCompany.c_activeDates.c_StrEndTime.second + distrib(sCM_genRand),
                         inObjCompany.c_activeDates.cAD_jobWeek));
    }

    std::mt19937 sCM_genRand;

    uint64_t sCM_cTotalCivil = 0;

    std::shared_ptr<sgRoadsMain> sCM_sRoadsMain;
    std::shared_ptr<groupLand> sCM_groupLand;
    std::shared_ptr<groupEconomy> sCM_groupEconomy;
    std::shared_ptr<sCivilStorage> sCM_civilStorage;
};

#endif //CITYOFWEIRDFISHES_SCIVILMAIN_H

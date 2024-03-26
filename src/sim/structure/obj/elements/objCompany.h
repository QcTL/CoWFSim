//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_OBJCOMPANY_H
#define CITYOFWEIRDFISHES_OBJCOMPANY_H


#include <cstdint>
#include <utility>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <unordered_set>
#include "contracts/cObjContracts.h"
#include "../../../behaviour/company/code/sCodeStoratge.h"

std::vector<std::string> vSyllablesP = {"am", "ca", "mi", "o", "ul", "er", "es", "pin", "tu", "ra", "ta", "la", "dro",
                                        "me", "dia", "mart", "sen", "ti", "ments", "tran", "qui", "li", "tat", "pen",
                                        "sa", "ment", "u", "su", "al", "ment", "ar", "ma", "ri"};
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> numStringsDist(2, 4);

class objCompany {
public:
    struct objComp_activeDates {
        uint8_t cAD_jobWeek;
        std::pair<uint32_t, uint32_t> c_StrEndTime;

        objComp_activeDates(const std::vector<bool> &cAdJobWeek, const std::pair<uint32_t, uint32_t> &cStrEndTime)
                : c_StrEndTime(cStrEndTime) {
            if (cAdJobWeek.size() != 7)
                throw std::invalid_argument("The input bool vector must have exactly 7 elements.");

            cAD_jobWeek = 0;
            for (size_t i = 0; i < 7; ++i)
                if (cAdJobWeek[i])
                    cAD_jobWeek |= (1u << i);
        }
    };

    explicit objCompany(uint32_t cUuid, const std::list<std::pair<int, int>> &ownStart, uint8_t typeCellStart,
                        objComp_activeDates activeDates)
            : c_uuid(cUuid), c_activeDates(std::move(activeDates)) {

        addAvailableLocation(ownStart, typeCellStart);
        int numStrings = numStringsDist(gen);
        std::vector<std::string> selectedStrings;
        std::sample(vSyllablesP.begin(), vSyllablesP.end(),
                    std::back_inserter(selectedStrings), numStrings, gen);

        for (const auto &str: selectedStrings)
            nName += str;
        nName[0] = toupper(nName[0]);
    }


    void addAvailableLocation(const std::list<std::pair<int, int>> &ctLCells, uint8_t typeCell) {
        c_cActiveLocations.insert(
                c_cActiveLocations.end(), ctLCells.begin(), ctLCells.end());
        if (c_cAvailableByType.find(typeCell) != c_cAvailableByType.end())
            c_cAvailableByType[typeCell] += 1;
        else
            c_cAvailableByType[typeCell] = 1;
    }


    void removeAvailableLocation(const std::list<std::pair<int, int>> &ctLCells, uint8_t typeCell) {
        for (const auto &item: ctLCells) {
            c_cActiveLocations.erase(
                    std::remove(c_cActiveLocations.begin(), c_cActiveLocations.end(), item),
                    c_cActiveLocations.end());
        }
        c_cAvailableByType[typeCell] -= 1;
    }

    uint32_t c_uuid{};
    std::string nName;

    std::pair<int, int> getOwnedByIndex(uint32_t lIndex) {
        return *std::next(c_cActiveLocations.begin(),
                          lIndex % (uint32_t) c_cActiveLocations.size()); // Advance iterator by 2 positions
    }

    std::map<uint32_t, int> c_pOwn;
    std::list<std::pair<int, int>> c_cActiveLocations;
    std::map<uint8_t, int> c_cAvailableByType;
    std::list<std::pair<int, int>> c_cRentedLocations;
    objComp_activeDates c_activeDates;

    uint32_t c_nEmployee = 0;
    double c_cActiveFunds = 0;
    float c_objFortnight = 0;
    float c_objYear = 0;
    float c_objMonth = 0;

    std::shared_ptr<sCodeStorage::sCodeObj> c_cCode;
};

#endif //CITYOFWEIRDFISHES_OBJCOMPANY_H

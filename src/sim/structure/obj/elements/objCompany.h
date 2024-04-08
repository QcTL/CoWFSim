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
#include "../../../eyeCatcher/eyeValue.h"

std::vector<std::string> vSyllablesP = {"am", "ca", "mi", "o", "ul", "er", "es", "pin", "tu", "ra", "ta", "la", "dro",
                                        "me", "dia", "mart", "sen", "ti", "tran", "qui", "li", "tat", "pen",
                                        "sa", "ment", "u", "su", "al", "ar", "ma", "ri", "ja", "po", "nes"};
std::random_device oC_rd;
std::mt19937 oC_gen(oC_rd());
std::uniform_int_distribution<> numStringsDist(2, 3);

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

    explicit objCompany(const std::list<std::pair<int, int>> &ownStart, uint8_t typeCellStart,
                        objComp_activeDates activeDates)
            :c_activeDates(std::move(activeDates)) {

        addAvailableLocation(ownStart, typeCellStart);
        int numStrings = numStringsDist(oC_gen);
        std::vector<std::string> selectedStrings;
        std::sample(vSyllablesP.begin(), vSyllablesP.end(),
                    std::back_inserter(selectedStrings), numStrings, oC_gen);

        for (const auto &str: selectedStrings)
            nName += str;
        nName[0] = static_cast<char>(toupper(nName[0]));

        c_nEmployee = eyeValue<uint32_t>("oC_" + nName + "-nEmployees", 0);
        c_cActiveFunds = eyeValue<double>("oC_" + nName + "-activeFunds", 0);
        c_objWeek = eyeValue<float>("oC_" + nName + "-gainWeek", 0);
        c_objYear = eyeValue<float>("oC_" + nName + "-gainYear", 0);
        c_objMonth = eyeValue<float>("oC_" + nName + "-gainMonth", 0);
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

    void addContractToCompany(uint8_t inCType, uint64_t inCUuid) {
        if (c_activeContracts.find(inCType) != c_activeContracts.end())
            c_activeContracts[inCType].push_back(inCUuid);
        else
            c_activeContracts[inCType] = {inCUuid};
    }

    std::map<uint32_t, int> c_pOwn;
    std::list<std::pair<int, int>> c_cActiveLocations;
    std::map<uint8_t, int> c_cAvailableByType;
    std::list<std::pair<int, int>> c_cRentedLocations;
    objComp_activeDates c_activeDates;

    eyeValue<uint32_t> c_nEmployee;
    eyeValue<double> c_cActiveFunds;
    eyeValue<float> c_objWeek;
    eyeValue<float> c_objYear;
    eyeValue<float> c_objMonth;

    std::shared_ptr<sCodeStorage::sCodeObj> c_cCode;

    std::map<uint8_t, std::list<uint64_t>> c_activeContracts;
    bool c_attrCanEmployee = true;
};

#endif //CITYOFWEIRDFISHES_OBJCOMPANY_H

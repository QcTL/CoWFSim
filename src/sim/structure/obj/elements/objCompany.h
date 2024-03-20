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

    explicit objCompany(uint32_t cUuid, const std::list<std::pair<int, int>> &ownStart,
                        objComp_activeDates activeDates)
            : c_uuid(cUuid), c_cActiveLocations(ownStart), c_activeDates(std::move(activeDates)) {
        int numStrings = numStringsDist(gen);
        std::vector<std::string> selectedStrings;
        std::sample(vSyllablesP.begin(), vSyllablesP.end(),
                    std::back_inserter(selectedStrings), numStrings, gen);

        for (const auto &str: selectedStrings)
            nName += str;
        nName[0] = toupper(nName[0]);
    }

    uint32_t c_uuid{};
    std::string nName;

    std::pair<int, int> getOwnedByIndex(uint32_t lIndex) {
        return *std::next(c_cActiveLocations.begin(),
                          lIndex % (uint32_t) c_cActiveLocations.size()); // Advance iterator by 2 positions
    }

    std::map<uint32_t, int> c_pOwn;
    std::list<std::pair<int, int>> c_cActiveLocations;
    std::list<std::pair<int, int>>  c_cRentedLocations;
    objComp_activeDates c_activeDates;

    double c_cActiveFunds = 0;
    float c_objFortnight = 0;
    float c_objYear = 0;
    float c_objMonth = 0;
};

#endif //CITYOFWEIRDFISHES_OBJCOMPANY_H

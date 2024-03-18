//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMMON_H
#define CITYOFWEIRDFISHES_SCOMMON_H


#include <cstdint>
#include <utility>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <random>
#include "elements/contracts/cObjContracts.h"

std::vector<std::string> vSyllablesP = {"am", "ca", "mi", "o", "ul", "er", "es", "pin", "tu", "ra", "ta", "la", "dro",
                                        "me", "dia", "mart", "sen", "ti", "ments", "tran", "qui", "li", "tat", "pen",
                                        "sa", "ment", "u", "su", "al", "ment", "ar", "ma", "ri"};
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> numStringsDist(2, 4);

struct objCompany {
    uint32_t c_uuid{};
    std::string nName;
    std::vector<std::pair<int, int>> c_cOwn;
    std::list<uint32_t> c_lUuidRentedSelf;
    std::list<uint32_t> c_lUuidRentedOther;
    std::map<uint32_t, int> c_pOwn;

    std::pair<uint32_t, uint32_t> c_StrEndTime;
    std::vector<uint64_t> c_cCode;

    objCompany() = default;
    explicit objCompany(uint32_t cUuid, const std::vector<std::pair<int, int>> &ownStart) : c_uuid(cUuid),
                                                                                            c_cOwn(ownStart) {
        int numStrings = numStringsDist(gen);
        std::vector<std::string> selectedStrings;
        std::sample(vSyllablesP.begin(), vSyllablesP.end(),
                    std::back_inserter(selectedStrings), numStrings, gen);

        for (const auto &str: selectedStrings)
            nName += str;
        nName[0] = toupper(nName[0]);
        c_cCode = {13835058055282163712};
    }
};


struct objProdRecipe {
    std::uint32_t pr_idObjEnd;
    std::vector<std::uint16_t> pr_reqBuilding;
    std::vector<std::uint32_t> pr_reqProdId;
    std::uint32_t pr_reqTime;
};

#endif //CITYOFWEIRDFISHES_SCOMMON_H

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

struct objProdRecipe {
    std::uint32_t pr_idObjEnd;
    std::vector<std::uint16_t> pr_reqBuilding;
    std::vector<std::uint32_t> pr_reqProdId;
    std::uint32_t pr_reqTime;
};

#endif //CITYOFWEIRDFISHES_SCOMMON_H

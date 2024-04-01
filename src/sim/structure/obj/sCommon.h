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

struct retObjCompany{
    std::vector<std::pair<int,int>> gCompVec;
    uint8_t gType;

    retObjCompany(const std::vector<std::pair<int, int>> &gCompVec, uint8_t gType) : gCompVec(gCompVec), gType(gType) {}
};

struct retObjSLayerCells {
    std::shared_ptr<gIGrid<uint8_t>> gUnderground;
    std::vector<retObjCompany> gCompanyPositions;
    std::vector<std::vector<std::pair<int, int>>> routesMetro;

};


#endif //CITYOFWEIRDFISHES_SCOMMON_H

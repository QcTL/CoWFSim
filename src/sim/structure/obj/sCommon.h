//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMMON_H
#define CITYOFWEIRDFISHES_SCOMMON_H


struct objProdRecipe {
    std::uint32_t pr_idObjEnd;
    std::vector<std::uint16_t> pr_reqBuilding;
    std::vector<std::uint32_t> pr_reqProdId;
    std::uint32_t pr_reqTime;
};

struct retObjSLayerCells {
    std::shared_ptr<gIGrid<uint8_t>> gUnderground;
    std::vector<std::vector<std::pair<int, int>>> routesMetro;

};

struct objActiveRute {
    std::pair<int, int> c_RStart;
    std::pair<int, int> c_REnd;
    bool c_IsReversed;
    uint64_t c_uuid;
};

struct sCodeObj {
    std::vector<uint64_t> sCO_Code;
    uint32_t sCO_Score = 0;

    sCodeObj(const std::vector<uint64_t> &sCoCode, uint32_t sCoScore) : sCO_Code(sCoCode), sCO_Score(sCoScore) {}
};

#endif //CITYOFWEIRDFISHES_SCOMMON_H

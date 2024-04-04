//
// Created by ganymede on 3/30/24.
//

#ifndef CITYOFWEIRDFISHES_RGTERRAIN_H
#define CITYOFWEIRDFISHES_RGTERRAIN_H

#include <memory>
#include <map>
#include <vector>
#include "../sim/structure/grids/transformation/gBaseToBorderDetection.h"
#include "../common/r2BitDirection.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"
#include "../sim/structure/grids/transformation/gBaseToStartBuildings.h"
#include "../sim/snCommonAtr.h"


class rgTerrain {
public:
    explicit rgTerrain(uint32_t inGridSize) {
        gLayerCurStruct = std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(inGridSize, inGridSize, 0));
    };

    void addNewBuildingRender(const std::pair<int, int> &inBPos, uint8_t inGenType) {
        const char *_gVal = "00000000"; // AIXO HO HA DE FER EL rgTERRAIN des de sgTERRAIN
        switch (inGenType) {
            case 1:
                _gVal = "00010000";
                break;
            case 2:
                _gVal = "00010001";
                break;
            case 3:
                _gVal = "00010010";
                break;
            case 4:
            case 5:
                _gVal = "00010101";
                break;
            default:
                break;
        }
        gLayerCurStruct->set(inBPos, (((uint32_t) (uint8_t) strtol(_gVal, nullptr, 2)) << 24)
                                     + rand() % (2 + 1));
    }

    void loadRenderWithSimGrids(const std::shared_ptr<gIGrid<uint8_t>> &inGTypeGen,
                                const std::shared_ptr<gIGrid<uint8_t>> &inGTypeSoil) {
        //RIVER:
        std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> pRiver = gBaseToBorderDetection::generate<uint32_t>(
                gLayerCurStruct, {gBorderType::gBNonConnex, gBorderOutside::gIsGroup}, {},
                BasicTransformations::genMaskFromGrid(inGTypeSoil, {8}));

        for (const auto &map_element: pRiver) {
            for (const auto &elm: map_element.second) {
                //8 code to 4:
                uint8_t p = 0;

                if ((elm.second & (1 << 6)) && (elm.second & (1 << 1))) {
                    p = (elm.second & (1 << 4) && !(elm.second & (1 << 3))) ? 1 : (elm.second & (1 << 3) &&
                                                                                   !(elm.second & (1 << 4))) ? 2 : 0;
                } else if ((elm.second & (1 << 3)) && (elm.second & (1 << 4))) {
                    p = (elm.second & (1 << 1) && !(elm.second & (1 << 6))) ? 3 : (elm.second & (1 << 6) &&
                                                                                   !(elm.second & (1 << 1))) ? 4 : 0;
                } else if ((elm.second & (1 << 4)) && (elm.second & (1 << 1))) {
                    p = 5;
                } else if ((elm.second & (1 << 3)) && (elm.second & (1 << 1))) {
                    p = 6;
                } else if ((elm.second & (1 << 3)) && (elm.second & (1 << 6))) {
                    p = 7;
                } else if ((elm.second & (1 << 6)) && (elm.second & (1 << 4))) {
                    p = 8;
                }
                gLayerCurStruct->set({elm.first.second, elm.first.first},
                                     (((uint32_t) (uint8_t) strtol("110000", nullptr, 2)) << 24) + p);
            }
        }
        //END RIVER

        //ROADS
        uint32_t idTypeRoadBig = (((uint32_t) (uint8_t) strtol("100001", nullptr, 2)) << 24);
        uint32_t idTypeRoadSmall = (((uint32_t) (uint8_t) strtol("100000", nullptr, 2)) << 24);

        BasicTransformations::copyWhere(gLayerCurStruct, inGTypeGen, {{5, idTypeRoadBig},
                                                                      {6, idTypeRoadSmall}});

        std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint16_t>>> p =
                gBaseToBorderDetection::generateByTwoBits(gLayerCurStruct,
                                                          {gBorderType::gBNonConnex, gBorderOutside::gIsNotGroup},
                                                          {idTypeRoadSmall, idTypeRoadBig},
                                                          BasicTransformations::genMaskFromGrid(gLayerCurStruct,
                                                                                                {idTypeRoadBig,
                                                                                                 idTypeRoadSmall}));

        for (const auto &map_element: p) {
            for (const auto &elm: map_element.second) {
                //8 code to 4:
                uint8_t pVal2Bits = ((elm.second & (3 << 1 * 2)) >> 1 * 2) << 3 * 2
                                    | ((elm.second & (3 << 4 * 2)) >> 4 * 2) << 2 * 2
                                    | ((elm.second & (3 << 6 * 2)) >> 6 * 2) << 1 * 2
                                    | ((elm.second & (3 << 3 * 2)) >> 3 * 2);
                uint32_t typeActRoad = gLayerCurStruct->get({elm.first.second, elm.first.first});
                uint8_t typeCenter = inGTypeGen->get({elm.first.second, elm.first.first}) == 6 ? 1 : 2;

                gLayerCurStruct->set({elm.first.second, elm.first.first},
                                     typeActRoad +
                                     r2BitDirection::getIdBy8Code(pVal2Bits, typeCenter));
            }
        }
        //END ROADS

        //BUILDINGS
        gBaseToStartBuildings::renderBuildingFromType<uint32_t>(gLayerCurStruct, inGTypeGen, inGTypeSoil,
                                                                snCommonAtr::getFlagAtr("snCA_Seed"));
        //END BUILDINGS
    }

    std::shared_ptr<gIGrid<uint32_t>> gLayerCurStruct;
};

#endif //CITYOFWEIRDFISHES_RGTERRAIN_H

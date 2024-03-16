//
// Created by Laminar on 29/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SLAYERCELLS_H
#define CITYOFWEIRDFISHES_SLAYERCELLS_H

#include <cstdint>
#include <memory>
#include <map>

#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "../../structure/grids/transformation/gBaseToBorderDetection.h"
#include "../../structure/grids/transformation/gBasicTransformations.h"
#include "../../structure/grids/transformation/gBaseToPattern.h"
#include "../../structure/grids/transformation/gBaseToLineRoads.h"
#include "../../structure/grids/transformation/gBaseToField.h"
#include "../../structure/grids/transformation/gBaseToStartBuildings.h"
#include "../../structure/grids/transformation/gBaseToInertiaRoads.h"
#include "../../structure/grids/transformation/gBaseToIRF.h"
#include "../../structure/grids/transformation/gPointToNearestElem.h"
#include "../../../common/r2BitDirection.h"

class sLayerCells {
public:
    struct retObjSLayerCells {
        std::shared_ptr<gIGrid<uint32_t>> gMatrix;
        std::shared_ptr<gIGrid<uint8_t>> gUnderground;
        std::vector<std::vector<std::pair<int, int>>> gCompanyPositions;
        std::vector<std::vector<std::pair<int, int>>> routesMetro;
    };

    static retObjSLayerCells gen(
            uint32_t lSize, const std::shared_ptr<gIGrid<uint8_t>> &gTypeSoil,
            const std::shared_ptr<gIGrid<uint8_t>> &gTypeGen,
            const std::vector<std::pair<int, int>> &cClusters,
            const std::map<std::string, std::string> &mValues) {

        std::shared_ptr<gIGrid<uint32_t>> gCell =
                std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(lSize, lSize, 0));

        std::shared_ptr<gIGrid<uint8_t>> gUnderground =
                std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));

        //RIVER:
        std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> pRiver = gBaseToBorderDetection::generate<uint32_t>(
                gCell, {gBorderType::gBNonConnex, gBorderOutside::gIsGroup}, {},
                BasicTransformations::genMaskFromGrid(gTypeSoil, {TypeSoil_T1Obstacle}));
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
                gCell->set({elm.first.second, elm.first.first},
                           (((uint32_t) (uint8_t) strtol("110000", nullptr, 2)) << 24) + p);
            }
        }

        //ROADS:
        //INTRACITY;
        uint32_t idTypeRoadBig = (((uint32_t) (uint8_t) strtol("100001", nullptr, 2)) << 24);
        uint32_t idTypeRoadSmall = (((uint32_t) (uint8_t) strtol("100000", nullptr, 2)) << 24);


        BasicTransformations::copyWhere(gCell, gTypeGen, {{5, idTypeRoadBig},
                                                          {6, idTypeRoadSmall}});

        std::vector<gBaseToIRF::gPositionEscape> rNext = gBaseToIRF::gen<uint32_t>(gCell, gTypeGen, {lSize / 2, 0},
                                                                                   {0, 1}, idTypeRoadBig, 5, 30);

        if (mValues.at("Estructura_Ciutat") == "Graella") {
            gBaseToPattern<uint32_t> gBP(gCell,
                                         gBaseToPattern<uint32_t>::gPatternType::gBPBlobSquares,
                                         gBaseToPattern<uint32_t>::gPatternParameters(4, 6, 40, 40),
                                         idTypeRoadSmall,
                                         BasicTransformations::genMaskFromGrid(gTypeSoil,
                                                                               {TypeSoil_T1Urban, TypeSoil_T2Urban,
                                                                                TypeSoil_T3Urban, TypeSoil_T1Factory,
                                                                                TypeSoil_T2Factory}));

        } else if (mValues.at("Estructura_Ciutat") == "Radial") {
            gBaseToPattern<uint32_t> gBP(gCell,
                                         gBaseToPattern<uint32_t>::gPatternType::gBPSquares,
                                         gBaseToPattern<uint32_t>::gPatternParameters(4, 6, 40, 40),
                                         idTypeRoadSmall,
                                         BasicTransformations::genMaskFromGrid(gTypeSoil,
                                                                               {TypeSoil_T1Urban, TypeSoil_T2Urban,
                                                                                TypeSoil_T3Urban, TypeSoil_T1Factory,
                                                                                TypeSoil_T2Factory}));
        }

        BasicTransformations::copyWhere(gTypeGen, gCell, {{idTypeRoadBig,   5},
                                                          {idTypeRoadSmall, 6}});

        for (const gBaseToIRF::gPositionEscape rPerp: rNext) {
            gBaseToIRF::gen<uint32_t>(gCell, gTypeGen, rPerp.pPerpStart,
                                      rPerp.pPerpOrigin, idTypeRoadSmall, 6, 100);
        }

        //HOUSES:
        gBaseToStartBuildings::gen(gCell, gTypeSoil, gTypeGen,
                                   {TypeSoil_T1Urban, TypeSoil_T2Urban, TypeSoil_T3Urban,
                                    TypeSoil_T1Factory, TypeSoil_T2Factory},
                                   0);


        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gCell->rangeUse();
        for (int i = gRange.first.first; i <= gRange.first.second; ++i) {
            for (int j = gRange.second.first; j <= gRange.second.second; ++j) {
                int vDist = gPointToNearestElem::find<uint8_t>(gTypeSoil, {i, j}, TypeSoil_T1Obstacle, 8).first;
                if ((vDist == 6 || vDist == 5 || vDist == 4) &&
                    gCell->get(i, j) != idTypeRoadBig) {
                    gCell->set(i, j, idTypeRoadSmall);
                }
            }
        }

        BasicTransformations::copyWhere(gTypeGen, gCell, {{idTypeRoadBig,   5},
                                                          {idTypeRoadSmall, 6}});


        std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint16_t>>> p =
                gBaseToBorderDetection::generateByTwoBits(gCell,
                                                          {gBorderType::gBNonConnex, gBorderOutside::gIsNotGroup},
                                                          {idTypeRoadSmall, idTypeRoadBig},
                                                          BasicTransformations::genMaskFromGrid(gCell, {idTypeRoadBig,
                                                                                                        idTypeRoadSmall}));

        for (const auto &map_element: p) {
            for (const auto &elm: map_element.second) {
                //8 code to 4:
                uint8_t pVal2Bits = ((elm.second & (3 << 1 * 2)) >> 1 * 2) << 3 * 2
                                    | ((elm.second & (3 << 4 * 2)) >> 4 * 2) << 2 * 2
                                    | ((elm.second & (3 << 6 * 2)) >> 6 * 2) << 1 * 2
                                    | ((elm.second & (3 << 3 * 2)) >> 3 * 2);
                uint32_t typeActRoad = gCell->get({elm.first.second, elm.first.first});
                uint8_t typeCenter = gTypeGen->get({elm.first.second, elm.first.first}) == 6 ? 1 : 2;

                gCell->set({elm.first.second, elm.first.first},
                           typeActRoad +
                           r2BitDirection::getIdBy8Code(pVal2Bits, typeCenter));
            }
        }

        std::vector<std::vector<std::pair<int, int>>> rMetro;
        for (int i = 0; i < cClusters.size(); i++) {
            rMetro.push_back(
                    gBaseToLineRoads::givenTwoPoints<uint8_t>(gUnderground,
                                                              cClusters[i], cClusters[(i + 1) % cClusters.size()],
                                                              1));
            gUnderground->set(cClusters[i], 2);
        }
        //gUnderground->set({0, 0}, 1);
        //gUnderground->set({1, 0}, 1);
        //gUnderground->set({1, 1}, 1);
        //gUnderground->set({0, 1}, 1);

        std::map<uint8_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> pUnderground =
                gBaseToBorderDetection::generate<uint8_t>(gUnderground,
                                                          {gBorderType::gBNonConnex, gBorderOutside::gIsNotGroup},
                                                          {1, 2},
                                                          BasicTransformations::genMaskFromGrid(gUnderground, {1, 2}));
        for (const auto &map_element: pUnderground) {
            for (const auto &elm: map_element.second) {
                //8 code to 4:
                uint8_t pVal2Bits = ((elm.second & (1 << 1)) >> 1) << 3
                                    | ((elm.second & (1 << 4)) >> 4) << 2
                                    | ((elm.second & (1 << 6)) >> 6) << 1
                                    | ((elm.second & (1 << 3)) >> 3);
                if (gUnderground->get({elm.first.second, elm.first.first}) == 2) { pVal2Bits |= 1 >> 4; }
                gUnderground->set({elm.first.second, elm.first.first}, pVal2Bits);
            }
        }

        //FIELDS:
        // gBaseToField<uint32_t> gBFields(gCell, 0, BasicTransformations::genMaskFromGrid(gTypeSoil, {TypeSoil_T1Farm}));
        /*
        BasicTransformations::replaceValues(gCell,
                                            {{2, ((uint32_t) (uint8_t) strtol("00010000", nullptr, 2)) << 24}});
        BasicTransformations::replaceValues(gCell,
                                            {{1, ((uint32_t) (uint8_t) strtol("00010001", nullptr, 2)) << 24}});
        */

        return {gCell, gUnderground, {}, rMetro};
    }

};

#endif //CITYOFWEIRDFISHES_SLAYERCELLS_H

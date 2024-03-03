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

class sLayerCells {
public:
    static std::shared_ptr<gIGrid<uint32_t>> gen(
            uint32_t lSize, const std::shared_ptr<gIGrid<uint8_t>> &gTypeSoil,
            const std::shared_ptr<gIGrid<uint8_t>> &gTypeGen,
            const std::map<std::string, std::string> &mValues) {
        std::shared_ptr<gIGrid<uint32_t>> gCell =
                std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(lSize, lSize, 0));

        //RIVER:
        std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> pRiver = gBaseToBorderDetection::generate<uint32_t>(
                gCell, {gBorderType::gBNonConnex, gBorderOutside::gIsGroup}, {},
                BasicTransformations::genMaskFromGrid(gTypeSoil, {5}));
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
        uint32_t idTypeRoadIntra = (((uint32_t) (uint8_t) strtol("100001", nullptr, 2)) << 24);
        uint32_t idTypeRoadOuter = (((uint32_t) (uint8_t) strtol("100000", nullptr, 2)) << 24);
        if (mValues.at("Estructura_Ciutat") == "Graella") {
            gBaseToPattern<uint32_t> gBP(gCell,
                                         gBaseToPattern<uint32_t>::gPatternType::gBPBlobSquares,
                                         gBaseToPattern<uint32_t>::gPatternParameters(5, 5, 20, 20),
                                         idTypeRoadIntra,
                                         BasicTransformations::genMaskFromGrid(gTypeSoil, {1, 2, 3}));

        } else if (mValues.at("Estructura_Ciutat") == "Radial") {
            gBaseToPattern<uint32_t> gBP(gCell,
                                         gBaseToPattern<uint32_t>::gPatternType::gBPSquares,
                                         gBaseToPattern<uint32_t>::gPatternParameters(5, 5, 20, 20),
                                         idTypeRoadIntra,
                                         BasicTransformations::genMaskFromGrid(gTypeSoil, {1, 2, 3}));
        }

        BasicTransformations::copyWhere(gTypeGen, gCell, {{idTypeRoadIntra, 2},
                                                          {idTypeRoadOuter, 2}});

        //HOUSES:
        gBaseToStartBuildings::gen(gCell, gTypeSoil, gTypeGen, {1, 2, 3}, 0);

        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gCell->rangeUse();
        for (int i = gRange.first.first; i <= gRange.first.second; ++i) {
            for (int j = gRange.second.first; j <= gRange.second.second; ++j) {
                int vDist = gPointToNearestElem::find<uint8_t>(gTypeSoil, {i, j}, 5, 8).first;
                if ((vDist == 6 || vDist == 5 || vDist == 4) &&
                    (gTypeSoil->get(i, j) == 0 || gTypeSoil->get(i, j) == 5 || gTypeSoil->get(i, j) == 4)) {
                    gCell->set(i, j, idTypeRoadOuter);
                }
            }
        }

        BasicTransformations::copyWhere(gTypeGen, gCell, {{idTypeRoadIntra, 2},
                                                          {idTypeRoadOuter, 2}});

        gBaseToIRF::gen<uint32_t>(gCell, gTypeGen, {50,
                                                    0}, {0, 1}, {idTypeRoadIntra, idTypeRoadOuter}, 20, 1);

        std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> p =
                gBaseToBorderDetection::generate(gCell, {gBorderType::gBNonConnex, gBorderOutside::gIsNotGroup},
                                                 {idTypeRoadOuter, idTypeRoadIntra},
                                                 BasicTransformations::genMaskFromGrid(gCell, {idTypeRoadOuter,
                                                                                               idTypeRoadIntra}));

        for (const auto &map_element: p) {
            for (const auto &elm: map_element.second) {
                //8 code to 4:
                uint8_t p = ((elm.second & (1 << 1)) != 0) << 3
                            | ((elm.second & (1 << 4)) != 0) << 2
                            | ((elm.second & (1 << 6)) != 0) << 1
                            | ((elm.second & (1 << 3)) != 0);
                gCell->set({elm.first.second, elm.first.first}, gCell->get({elm.first.second, elm.first.first}) + p);
            }
        }

        //FIELDS:
        gBaseToField<uint32_t> gBFields(gCell, 0, BasicTransformations::genMaskFromGrid(gTypeSoil, {4}));

        /*
        BasicTransformations::replaceValues(gCell,
                                            {{2, ((uint32_t) (uint8_t) strtol("00010000", nullptr, 2)) << 24}});
        BasicTransformations::replaceValues(gCell,
                                            {{1, ((uint32_t) (uint8_t) strtol("00010001", nullptr, 2)) << 24}});
        */




        return gCell;
    }

};

#endif //CITYOFWEIRDFISHES_SLAYERCELLS_H

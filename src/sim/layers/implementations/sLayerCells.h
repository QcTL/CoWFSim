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
    static retObjSLayerCells gen(
            uint32_t lSize, const std::shared_ptr<sgTerrain> &sgTerrain,
            const std::vector<std::pair<int, int>> &cClusters,
            const std::map<std::string, std::string> &mValues, int inSeed = 0) {

        std::shared_ptr<gIGrid<uint8_t>> gUnderground =
                std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));

        //ROADS:
        //INTRACITY;
        std::vector<gBaseToIRF::gPositionEscape> rNext = gBaseToIRF::gen<uint8_t>(sgTerrain->gTG_TypeGen,
                                                                                  {lSize / 2, 0}, {0, 1},
                                                                                  5, 30, inSeed);

        if (mValues.at("Estructura_Ciutat") == "Graella") {
            gBaseToPattern<uint8_t> gBP(sgTerrain->gTG_TypeGen,
                                        gBaseToPattern<uint8_t>::gPatternType::gBPBlobSquares,
                                        gBaseToPattern<uint8_t>::gPatternParameters(4, 6, 40, 40),
                                        sgTerrain::sgT_TypeGen::sgT_TG_RoadS,
                                        BasicTransformations::genMaskFromGrid(sgTerrain->gTG_TypeSoil,
                                                                              {TypeSoil_T1Urban, TypeSoil_T2Urban,
                                                                               TypeSoil_T3Urban, TypeSoil_T1Factory,
                                                                               TypeSoil_T2Factory}),
                                        inSeed);

        } else if (mValues.at("Estructura_Ciutat") == "Radial") {
            gBaseToPattern<uint8_t> gBP(sgTerrain->gTG_TypeGen,
                                        gBaseToPattern<uint8_t>::gPatternType::gBPSquares,
                                        gBaseToPattern<uint8_t>::gPatternParameters(4, 6, 40, 40),
                                        sgTerrain::sgT_TypeGen::sgT_TG_RoadS,
                                        BasicTransformations::genMaskFromGrid(sgTerrain->gTG_TypeSoil,
                                                                              {TypeSoil_T1Urban, TypeSoil_T2Urban,
                                                                               TypeSoil_T3Urban, TypeSoil_T1Factory,
                                                                               TypeSoil_T2Factory}),
                                        inSeed);
        }

        for (const gBaseToIRF::gPositionEscape rPerp: rNext) {
            gBaseToIRF::gen<uint8_t>(sgTerrain->gTG_TypeGen, rPerp.pPerpStart,
                                     rPerp.pPerpOrigin, sgTerrain::sgT_TypeGen::sgT_TG_RoadS, 100, inSeed);
        }


        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = sgTerrain->gTG_TypeGen->rangeUse();
        for (int i = gRange.first.first; i <= gRange.first.second; ++i) {
            for (int j = gRange.second.first; j <= gRange.second.second; ++j) {
                int vDist = gPointToNearestElem::find<uint8_t>(sgTerrain->gTG_TypeSoil, {i, j}, TypeSoil_T1Obstacle,
                                                               8).first;
                if ((vDist == 6 || vDist == 5 || vDist == 4) &&
                    sgTerrain->gTG_TypeGen->get(i, j) != sgTerrain::sgT_TypeGen::sgT_TG_RoadB) {
                    sgTerrain->gTG_TypeGen->set(i, j, sgTerrain::sgT_TypeGen::sgT_TG_RoadS);
                }
            }
        }

        //HOUSES:
        gBaseToStartBuildings::gen(sgTerrain->gTG_TypeSoil, sgTerrain->gTG_TypeGen,
                                              {TypeSoil_T1Urban, TypeSoil_T2Urban, TypeSoil_T3Urban,
                                               TypeSoil_T1Factory, TypeSoil_T2Factory},
                                              {5, 6}, inSeed);

        //METRO:

        std::vector<std::vector<std::pair<int, int>>> rMetro;
        for (int i = 0; i < cClusters.size(); i++) {
            rMetro.push_back(
                    gBaseToLineRoads::givenTwoPoints<uint8_t>(gUnderground,
                                                              cClusters[i], cClusters[(i + 1) % cClusters.size()],
                                                              1));
            gUnderground->set(cClusters[i], 2);
        }

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
        // gBaseToField<uint32_t> gBFields(gCell, 0, BasicTransformations::genMaskFromGrid(gTypeSoil, {TypeSoil_T1Farm}), inSeed);
        /*
        BasicTransformations::replaceValues(gCell,
                                            {{2, ((uint32_t) (uint8_t) strtol("00010000", nullptr, 2)) << 24}});
        BasicTransformations::replaceValues(gCell,
                                            {{1, ((uint32_t) (uint8_t) strtol("00010001", nullptr, 2)) << 24}});
        */

        return {gUnderground, rMetro};
    }

};

#endif //CITYOFWEIRDFISHES_SLAYERCELLS_H

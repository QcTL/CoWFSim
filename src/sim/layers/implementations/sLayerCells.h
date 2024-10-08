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

    /**
     * @fn retObjSLayerCells gen
     * @brief Modifies the terrain matrix to comply with the attributes given at the start of the simulation
     * @param lSize The size of the total grid
     * @param sgTerrain The matrix where the modification will apply
     * @param cClusters A vector with the centers of the active zones, like civilian or factories
     * @param mValues A map containing all the attributes that are defined by the user
     * @param inSeed The value of the seed which random systems will be based
     * @return A struct containing the matrix of the underground and the route of the metro
     */
    static retObjSLayerCells gen(
            uint32_t lSize, const std::shared_ptr<sgTerrain> &sgTerrain,
            const std::vector<std::pair<int, int>> &cClusters,
            const std::map<std::string, std::string> &mValues, int inSeed = 0) {

        std::shared_ptr<gIGrid<uint8_t>> gUnderground =
                std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));

        //ROADS:
        //INTRACITY;
        struct sPosStartMainRoad {
            std::pair<int, int> sPSM_StartPos;
            std::pair<int, int> sPSM_StartDir;
        };
        std::vector<sPosStartMainRoad> sVecMainRoads = {
                {{lSize / 2,             0},                     {0,  1}},
                {{0,                     lSize / 2},             {1,  0}},
                {{lSize / 2 + lSize / 3, lSize - 1},             {0,  -1}},
                {{lSize - 1,             lSize / 2 + lSize / 3}, {-1, 0}},
        };
        std::vector<int> sDispersionByNRoads = {30, 60, 100, 150};

        std::vector<gBaseToIRF::gPositionEscape> rNext;
        for (int i = 0; i < std::stoi(mValues.at("Quanitat_Carrers_Princiapls")); i++) {
            std::vector<gBaseToIRF::gPositionEscape> gNewVec = gBaseToIRF::gen<uint8_t>(sgTerrain->gTG_TypeGen,
                                                                                        sVecMainRoads[i].sPSM_StartPos,
                                                                                        sVecMainRoads[i].sPSM_StartDir,
                                                                                        5, sDispersionByNRoads[i],
                                                                                        false, inSeed);
            rNext.insert(rNext.end(), gNewVec.begin(), gNewVec.end());
        }

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
                                     rPerp.pPerpOrigin, sgTerrain::sgT_TypeGen::sgT_TG_RoadS, 255,
                                     std::stoi(mValues.at("Quanitat_Carrers_Princiapls")) > 2, inSeed);
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

        //HOUSES && BUILDINGS:
        int levelPopulation = 1;
        if (mValues.at("Nivell_Poblacio_Inicial") == "Petita")
            levelPopulation = 3;
        else if (mValues.at("Nivell_Poblacio_Inicial") == "Mitjana")
            levelPopulation = 2;
        else if (mValues.at("Nivell_Poblacio_Inicial") == "Gran")
            levelPopulation = 1;
        gBaseToStartBuildings::gen(sgTerrain->gTG_TypeSoil, sgTerrain->gTG_TypeGen,
                                   {TypeSoil_T1Urban, TypeSoil_T2Urban, TypeSoil_T3Urban,
                                    TypeSoil_T1Factory, TypeSoil_T2Factory},
                                   {5, 6}, levelPopulation, inSeed);

        //FIELDS:

        sgTerrain->gTG_fieldsBlobPositions = gBaseToField<uint8_t>::genMapPos(
                sgTerrain->gTG_TypeGen,
                BasicTransformations::genMaskFromGrid(sgTerrain->gTG_TypeSoil, {TypeSoil_T1Farm}), inSeed);
        for (const auto &mapElem: sgTerrain->gTG_fieldsBlobPositions) {
            for (const auto &vecElem: mapElem.second) {
                sgTerrain->gTG_TypeGen->set({vecElem.first.second, vecElem.first.first},
                                            sgTerrain::sgT_TypeGen::sgT_TG_FieldBuilding);
            }
        }

        //METRO:

        std::vector<std::vector<std::pair<int, int>>> rMetro;
        for (int i = 0; i < cClusters.size(); i++) {
            rMetro.push_back(
                    gBaseToLineRoads::givenTwoPoints<uint8_t>(gUnderground,
                                                              cClusters[i], cClusters[(i + 1) % cClusters.size()],
                                                              1));
        }

        for (const auto & cCluster : cClusters) {
            gUnderground->set(cCluster, 2);
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
                if (gUnderground->get({elm.first.second, elm.first.first}) == 2) { pVal2Bits = 16; }
                gUnderground->set({elm.first.second, elm.first.first}, pVal2Bits);
            }
        }

        return {gUnderground, rMetro};
    }

};

#endif //CITYOFWEIRDFISHES_SLAYERCELLS_H

//
// Created by Laminar on 29/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SLAYERTYPE_H
#define CITYOFWEIRDFISHES_SLAYERTYPE_H

#include <cstdint>
#include <memory>
#include <map>
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "../../structure/grids/transformation/gBaseToRiver.h"
#include "../../structure/grids/transformation/gBaseClosestToPoint.h"
#include "../../structure/grids/transformation/gBasicTransformations.h"
#include "../../structure/grids/transformation/gPointToNearestElem.h"

const uint8_t TypeSoil_Nothing = 0;
const uint8_t TypeSoil_T1Urban = 1;
const uint8_t TypeSoil_T2Urban = 2;
const uint8_t TypeSoil_T3Urban = 3;
const uint8_t TypeSoil_T1Factory = 4;
const uint8_t TypeSoil_T2Factory = 5;
const uint8_t TypeSoil_T1Farm = 6;
const uint8_t TypeSoil_T1Protected = 7;
const uint8_t TypeSoil_T1Obstacle = 8;
const uint8_t TypeSoil_T2Obstacle = 9;

class sLayerType {
public:

    static double distance(const std::pair<int, int> &p1, const std::pair<int, int> &p2) {
        return std::sqrt(std::pow(p2.first - p1.first, 2) + std::pow(p2.second - p1.second, 2));
    }

    static std::vector<std::pair<int, int>> nearestNeighbor(const std::vector<std::pair<int, int>> &points) {
        std::vector<std::pair<int, int>> result;
        std::vector<bool> visited(points.size(), false);

        // Start from the first point
        int current = 0;
        result.push_back(points[current]);
        visited[current] = true;

        // Visit nearest neighbors until all points are visited
        while (result.size() < points.size()) {
            double minDist = std::numeric_limits<double>::max();
            int nextIndex = -1;

            // Find the nearest unvisited neighbor
            for (int i = 0; i < points.size(); ++i) {
                if (!visited[i]) {
                    double dist = distance(points[current], points[i]);
                    if (dist < minDist) {
                        minDist = dist;
                        nextIndex = i;
                    }
                }
            }

            // Move to the nearest unvisited neighbor
            if (nextIndex != -1) {
                current = nextIndex;
                result.push_back(points[current]);
                visited[current] = true;
            }
        }

        return result;
    }

    struct returnLayerType {
        std::shared_ptr<gIGrid<uint8_t>> genTypeSoil;
        std::vector<std::pair<int, int>> centerClusters;
    };

    static returnLayerType gen(uint32_t lSize, const std::shared_ptr<gIGrid<uint8_t>> &gTypeGen,
                               const std::map<std::string, std::string> &mValues, const int inSeed = 0) {
        std::shared_ptr<gIGrid<uint8_t>> gLayerTypeSoil =
                std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, TypeSoil_Nothing));
        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gLayerTypeSoil->rangeUse();

        float lSizeRiver = 0;
        if (mValues.at("Mida_Riu") == "Petita")
            lSizeRiver = 2;
        else if (mValues.at("Mida_Riu") == "Mitjana")
            lSizeRiver = 2.25;
        else if (mValues.at("Mida_Riu") == "Gran")
            lSizeRiver = 2.5;
        else if (mValues.at("Mida_Riu") == "Molt_Gran")
            lSizeRiver = 2.75;

        if (mValues.at("Conte_Riu") == "on")
            gBaseToRiver<uint8_t>::generate(gLayerTypeSoil, 20, lSizeRiver, TypeSoil_T1Obstacle);

        std::mt19937 sLT_genRVal;
        if (inSeed == 0)
            sLT_genRVal.seed(std::time(nullptr));
        else
            sLT_genRVal.seed(inSeed);


        //NUCLEUS CITY
        struct sLT_typeNucleusSize {
            int pRadiusC;
            int pRadiusE;
            int pRadiusQ;
        };
        std::vector<sLT_typeNucleusSize> tSizeNucleusCiv = {{3, 5,  9},
                                                            {6, 9,  13},
                                                            {9, 12, 17}};
        int selSizeNucleus = 0;
        if (mValues.at("Centres_Urbans_Mida") == "Petita")
            selSizeNucleus = 0;
        else if (mValues.at("Centres_Urbans_Mida") == "Mitjana")
            selSizeNucleus = 1;
        else if (mValues.at("Centres_Urbans_Mida") == "Gran")
            selSizeNucleus = 2;
        std::vector<std::pair<int, int>> gCenterCoreUrban(std::stoi(mValues.at("Centres_Urbans")));
        std::uniform_int_distribution<int> distGCenter(0, (int) lSize);
        for (auto &gCenter: gCenterCoreUrban) {
            gCenter = {distGCenter(sLT_genRVal), distGCenter(sLT_genRVal)};
            genClusterTypeSoil(gCenter,
                               tSizeNucleusCiv[selSizeNucleus].pRadiusC,
                               tSizeNucleusCiv[selSizeNucleus].pRadiusE,
                               tSizeNucleusCiv[selSizeNucleus].pRadiusQ, gLayerTypeSoil);
        }

        //NUCLEUS INDUSTRIAL
        int selSizeNucleusInd = 0;
        if (mValues.at("Node_Industrial_Mida") == "Petita")
            selSizeNucleusInd = 0;
        else if (mValues.at("Node_Industrial_Mida") == "Mitjana")
            selSizeNucleusInd = 1;
        else if (mValues.at("Node_Industrial_Mida") == "Gran")
            selSizeNucleusInd = 2;
         std::vector<std::pair<int, int>> gCenterCoreIndustrial(std::stoi(mValues.at("Quanitat_Node_Industrial")));
        for (auto &gCenter: gCenterCoreIndustrial) {
            gCenter = {distGCenter(sLT_genRVal), distGCenter(sLT_genRVal)};
            genClusterTypeIndustry(gCenter, tSizeNucleusCiv[selSizeNucleusInd].pRadiusC,
                                   tSizeNucleusCiv[selSizeNucleusInd].pRadiusE, gLayerTypeSoil);
        }

        auto wMask = BasicTransformations::genMaskFromGrid(gLayerTypeSoil, {TypeSoil_Nothing});
        for (int i = gRange.first.first; i <= gRange.first.second; ++i) {
            for (int j = gRange.second.first; j <= gRange.second.second; ++j) {
                if (wMask->get(i, j)
                    && gPointToNearestElem::find<uint8_t>(gLayerTypeSoil, {i, j}, TypeSoil_T1Obstacle, 15).first > 0) {
                    gLayerTypeSoil->set(i, j, TypeSoil_T1Farm);
                }
            }
        }
        BasicTransformations::copyWhere(gTypeGen, gLayerTypeSoil,
                                        {
                                                {7,               5},
                                                {TypeSoil_T1Farm, 4}});


        gCenterCoreUrban.insert(gCenterCoreUrban.end(), gCenterCoreIndustrial.begin(), gCenterCoreIndustrial.end());

        //Reorder cClusters:
        std::vector<std::pair<int, int>> rP = nearestNeighbor(gCenterCoreUrban);
        return {gLayerTypeSoil, rP};
    }

private:
    static void genClusterTypeSoil(const std::pair<int, int> &pCenter,
                                   const uint32_t pRadiusC, const uint32_t pRadiusE, const uint32_t pRadiusQ,
                                   const std::shared_ptr<gIGrid<uint8_t>> &gLayerTypeSoil) {

        gBaseClosestToPoint::gen<uint8_t>(gLayerTypeSoil, {
                                                  {pCenter, pRadiusC},
                                          }, TypeSoil_T1Urban,
                                          BasicTransformations::genMaskFromGrid(gLayerTypeSoil,
                                                                                {TypeSoil_Nothing}));

        gBaseClosestToPoint::gen<uint8_t>(gLayerTypeSoil, {
                                                  {pCenter, pRadiusE},
                                          }, TypeSoil_T2Urban,
                                          BasicTransformations::genMaskFromGrid(gLayerTypeSoil, {TypeSoil_Nothing}));

        gBaseClosestToPoint::gen<uint8_t>(gLayerTypeSoil, {
                                                  {pCenter, pRadiusQ},
                                          }, TypeSoil_T3Urban,
                                          BasicTransformations::genMaskFromGrid(gLayerTypeSoil, {TypeSoil_Nothing}));
    }

    static void
    genClusterTypeIndustry(const std::pair<int, int> &pCenter,
                           const uint32_t pRadiusC, const uint32_t pRadiusE,
                           const std::shared_ptr<gIGrid<uint8_t>> &gLayerTypeSoil) {

        gBaseClosestToPoint::gen<uint8_t>(gLayerTypeSoil, {
                                                  {pCenter, pRadiusC},
                                          }, TypeSoil_T1Factory,
                                          BasicTransformations::genMaskFromGrid(gLayerTypeSoil, {TypeSoil_Nothing}));

        gBaseClosestToPoint::gen<uint8_t>(gLayerTypeSoil, {
                                                  {pCenter, pRadiusE},
                                          }, TypeSoil_T2Factory,
                                          BasicTransformations::genMaskFromGrid(gLayerTypeSoil, {TypeSoil_Nothing}));
    }

};

#endif //CITYOFWEIRDFISHES_SLAYERTYPE_H

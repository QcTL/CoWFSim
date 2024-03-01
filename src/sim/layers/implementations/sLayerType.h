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

class sLayerType {
public:
    static std::shared_ptr<gIGrid<uint8_t>> gen(uint32_t lSize, const std::map<std::string, std::string> &mValues) {
        std::shared_ptr<gIGrid<uint8_t>> gLayerTypeGen =
                std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gLayerTypeGen->rangeUse();

        float lSizeRiver = 0;
        if (mValues.at("Mida_Simulacio") == "Petita")
            lSizeRiver = 1;
        else if (mValues.at("Mida_Simulacio") == "Mitjana")
            lSizeRiver = 1.5;
        else if (mValues.at("Mida_Simulacio") == "Gran")
            lSizeRiver = 2;
        else if (mValues.at("Mida_Simulacio") == "Molt_Gran")
            lSizeRiver = 2.7;

        if (mValues.at("Conte_Riu") == "on")
            gBaseToRiver<uint8_t>::generate(gLayerTypeGen, 20, lSizeRiver, 5);

        gBaseClosestToPoint::gen<uint8_t>(gLayerTypeGen, {{{lSize / 2, lSize / 2}, 7}, {{30,40},7}}, 1,
                                          BasicTransformations::genMaskFromGrid(gLayerTypeGen, {0}));

        gBaseClosestToPoint::gen<uint8_t>(gLayerTypeGen, {{{lSize / 2, lSize / 2}, 15}, {{30,40},15}}, 2,
                                          BasicTransformations::genMaskFromGrid(gLayerTypeGen, {0}));

        gBaseClosestToPoint::gen<uint8_t>(gLayerTypeGen, {{{lSize / 2, lSize / 2}, 25}, {{30,40},25}}, 3,
                                          BasicTransformations::genMaskFromGrid(gLayerTypeGen, {0}));


        auto wMask = BasicTransformations::genMaskFromGrid(gLayerTypeGen, {0});
        for (int i = gRange.first.first; i <= gRange.first.second; ++i) {
            for (int j = gRange.second.first; j <= gRange.second.second; ++j) {
                if (wMask->get(i, j)
                    && gPointToNearestElem::find<uint8_t>(gLayerTypeGen, {i, j}, 5, 6).first > 0) {
                    gLayerTypeGen->set(i, j, 4);
                }
            }
        }

        return gLayerTypeGen;
    }

};

#endif //CITYOFWEIRDFISHES_SLAYERTYPE_H

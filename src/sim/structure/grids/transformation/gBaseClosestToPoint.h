//
// Created by Laminar on 29/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASECLOSESTTOPOINT_H
#define CITYOFWEIRDFISHES_GBASECLOSESTTOPOINT_H

#include <memory>
#include <vector>
#include "../gIGrid.h"
#include "cEspec/gHelpPerlinNoise.h"

#define DB_PERLIN_IMPL
#include "cEspec/db_perlin.hpp"

class gBaseClosestToPoint {
public:
    struct pDefinition {
        std::pair<uint32_t, uint32_t> pPos;
        uint32_t pRadius;
    };

    template<typename T>
    static void gen(const std::shared_ptr<gIGrid<T>> &gRes, const std::vector<pDefinition> &vPoints,
                    const T endValue, const std::shared_ptr<gIGrid<bool>> &pMask = nullptr) {
        bool hasMask(pMask != nullptr);

        PerlinNoise pN;

        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gRes->rangeUse();
        double GWidth = (gRange.first.second - gRange.first.first) + 1;
        double GHeight = (gRange.second.second - gRange.second.first) + 1;
        for (int i = gRange.first.first; i <= gRange.first.second; ++i) {
            for (int j = gRange.second.first; j <= gRange.second.second; ++j) {
                if (!hasMask || pMask->get(i, j)) {
                    for (const pDefinition &pD: vPoints) {
                        if ((i - pD.pPos.first) * (i - pD.pPos.first) +
                            (j - pD.pPos.second) * (j - pD.pPos.second) +
                            (db::perlin(((double)i/GWidth)*25,((double)j/GHeight)*25)) * 60
                            < pD.pRadius * pD.pRadius) {
                            gRes->set(i, j, endValue);
                            break;
                        }
                    }
                }
            }
        }

    }
};

#endif //CITYOFWEIRDFISHES_GBASECLOSESTTOPOINT_H

//
// Created by Laminar on 28/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASICTRANSFORMATIONS_H
#define CITYOFWEIRDFISHES_GBASICTRANSFORMATIONS_H

#include <memory>
#include <vector>
#include "../gIGrid.h"

class BasicTransformations {

public:

    template<typename T>
    static void GaussianFilter(std::shared_ptr<gIGrid<T>> grid) {
        std::vector<T> nAttributes;
        std::pair<std::pair<int, int>, std::pair<int, int>> range = grid->rangeUse();

        static std::vector<T> dValues = {0.125, 0.125, 0.125, 0.125, 0.0625, 0.0625, 0.0625, 0.0625};
        for (int i = range.second.first; i < (range.second.second + 1); i++) {
            for (int j = range.first.first; j < (range.first.second + 1); j++) {
                std::vector<std::pair<int, int>> dOffsets =
                        {{i + 1, j},
                         {i - 1, j},
                         {i,     j + 1},
                         {i,     j - 1},
                         {i + 1, j + 1},
                         {i - 1, j + 1},
                         {i + 1, j - 1},
                         {i - 1, j - 1}};
                T r = 0.25 * grid->get(i, j);
                for (int k = 0; k < dOffsets.size(); k++) {
                    if (grid->isInside(dOffsets[k].first, dOffsets[k].second)) {
                        r += dValues[k] * grid->get(dOffsets[k].first, dOffsets[k].second);
                    }
                }
                nAttributes.push_back(r);
            }
        }

        int width = range.first.second - range.first.first + 1;

        for (int i = 0; i < nAttributes.size(); i++) {
            int x = range.first.first + i % width;
            int y = range.second.first + i / width;
            grid->set(y, x, nAttributes[i]);
        }
    }

};

#endif //CITYOFWEIRDFISHES_GBASICTRANSFORMATIONS_H

//
// Created by Laminar on 17/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOLINEROADS_H
#define CITYOFWEIRDFISHES_GBASETOLINEROADS_H

#include <cstdint>
#include <memory>
#include <cmath>
#include "../gIGrid.h"

template<typename T>
class gBaseToLineRoads {
private:
    bool hasMask;
public:
    gBaseToLineRoads(std::shared_ptr<gIGrid<T>> gGrid, float dSteep, uint32_t dHeight,
                     std::shared_ptr<gIGrid<bool>> gMask = nullptr) : hasMask(gMask != nullptr) {
        auto rSize = gGrid->rangeUse();
        int GWidth = (rSize.first.second - rSize.first.first) + 1;

        for (int j = 0; j < GWidth; j++) {
            uint32_t yPre = std::round(dSteep * j + dHeight);
            uint32_t yPost = std::round(dSteep * (j + 1) + dHeight);

            for (uint32_t k = yPre; k <= yPost; k++) {
                if (gGrid->isInside(j, k) && (!hasMask || gMask->get(j, k))) {
                    gGrid->set(j, k, 3);
                }
            }
        }
    }

};

#endif //CITYOFWEIRDFISHES_GBASETOLINEROADS_H

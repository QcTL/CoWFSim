//
// Created by Laminar on 17/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOLINEROADS_H
#define CITYOFWEIRDFISHES_GBASETOLINEROADS_H

#include <cstdint>
#include <memory>
#include <cmath>
#include "../gIGrid.h"


class gBaseToLineRoads {
private:
    static bool hasMask;
public:

    template<typename T>
    static void givenFunction(std::shared_ptr<gIGrid<T>> gGrid, float dSteep, uint32_t dHeight,
                              std::shared_ptr<gIGrid<bool>> gMask = nullptr) {
        hasMask = gMask != nullptr;
        auto rSize = gGrid->rangeUse();
        int GWidth = (rSize.first.second - rSize.first.first) + 1;

        for (int j = 0; j < GWidth; j++) {
            int yPre = std::round(dSteep * j + dHeight);
            int yPost = std::round(dSteep * (j + 1) + dHeight);
            int mMax = std::max(yPre, yPost);
            int mMin = std::min(yPre, yPost);

            for (int k = mMin; k <= mMax; k++) {
                if (gGrid->isInside(j, k) && (!hasMask || gMask->get(j, k))) {
                    gGrid->set(j, k, 4);
                }
            }
        }
    }

    template<typename T>
    static void givenTwoPoints(std::shared_ptr<gIGrid<T>> gGrid,
                               std::pair<int, int> pStart,
                               std::pair<int, int> pEnd,
                               std::shared_ptr<gIGrid<bool>> gMask = nullptr) {
        hasMask = gMask != nullptr;

        double dSteep = (pEnd.first - pStart.first) != 0 ?  (pEnd.second - pStart.second) /(pEnd.first - pStart.first) : 0;
        double dHeight = pStart.second - dSteep * pStart.first;

        for (int j = pStart.first; j < pEnd.first; j++) {
            int yPre = std::round(dSteep * j + dHeight);
            int yPost = std::round(dSteep * (j + 1) + dHeight);
            int mMax = std::max(yPre, yPost);
            int mMin = std::min(yPre, yPost);

            for (int k = mMin; k <= mMax; k++) {
                if (gGrid->isInside(j, k) && (!hasMask || gMask->get(j, k))) {
                    gGrid->set(j, k, 4);
                }
            }
        }

        gGrid->set(pStart, 4);
        gGrid->set(pEnd, 4);
    }

};
bool gBaseToLineRoads::hasMask;

#endif //CITYOFWEIRDFISHES_GBASETOLINEROADS_H

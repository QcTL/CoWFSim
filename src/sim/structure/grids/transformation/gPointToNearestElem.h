//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GPOINTTONEARESTELEM_H
#define CITYOFWEIRDFISHES_GPOINTTONEARESTELEM_H

#include <memory>
#include <numeric>
#include <vector>
#include "../gIGrid.h"

class gPointToNearestElem {
public:
    template<typename T>
    static std::pair<uint32_t, std::pair<int, int>>
    find(const std::shared_ptr<gIGrid<T>> &gRef, const std::pair<int, int> rPos, T toBeNear,
                uint32_t maxCap) {

        int i = rPos.first;
        int j = rPos.second;

        uint32_t dDist = 1;
        while (dDist < maxCap) {
            if (dDist % 2 != 0) {
                int gValid = (dDist + 1) / 2;
                if (gRef->isInside(i + gValid, j) && gRef->get(i + gValid, j) == toBeNear)
                    return {dDist, {i + gValid, j}};
                else if (gRef->isInside(i - gValid, j) && gRef->get(i - gValid, j) == toBeNear)
                    return {dDist, {i - gValid, j}};
                else if (gRef->isInside(i, j + gValid) && gRef->get(i, j + gValid) == toBeNear)
                    return {dDist, {i, j + gValid}};
                else if (gRef->isInside(i, j - gValid) && gRef->get(i, j - gValid) == toBeNear)
                    return {dDist, {i, j - gValid}};
            } else {
                int gValid = dDist / 2;
                std::vector<int> vec(gValid);
                std::vector<int> vecRev(gValid);

                std::iota(vec.begin(), vec.end(), 1);
                std::iota(vecRev.rbegin(), vecRev.rend(), 1);
                for (int k = 0; k < vec.size(); k++) {
                    if (gRef->isInside(i + vec[k], j + vecRev[k]) && gRef->get(i + vec[k], j + vecRev[k]) == toBeNear)
                        return {dDist, {i + vec[k], j + vecRev[k]}};
                    if (gRef->isInside(i - vec[k], j + vecRev[k]) && gRef->get(i - vec[k], j + vecRev[k]) == toBeNear)
                        return {dDist, {i - vec[k], j + vecRev[k]}};
                    if (gRef->isInside(i + vec[k], j - vecRev[k]) && gRef->get(i + vec[k], j - vecRev[k]) == toBeNear)
                        return {dDist, {i + vec[k], j - vecRev[k]}};
                    if (gRef->isInside(i - vec[k], j - vecRev[k]) && gRef->get(i - vec[k], j - vecRev[k]) == toBeNear)
                        return {dDist, {i - vec[k], j - vecRev[k]}};
                }
            }
            dDist++;
        }
        return {0, {0, 0}};
    }

};

#endif //CITYOFWEIRDFISHES_GPOINTTONEARESTELEM_H

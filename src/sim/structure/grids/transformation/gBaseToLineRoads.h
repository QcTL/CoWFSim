//
// Created by Laminar on 17/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOLINEROADS_H
#define CITYOFWEIRDFISHES_GBASETOLINEROADS_H

#include <cstdint>
#include <memory>
#include <cmath>
#include <set>
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
    static std::vector<std::pair<int, int>> givenTwoPoints(std::shared_ptr<gIGrid<T>> gGrid,
                                                           std::pair<int, int> pStart, std::pair<int, int> pEnd,
                                                           T endValue,
                                                           std::shared_ptr<gIGrid<bool>> gMask = nullptr) {
        hasMask = gMask != nullptr;
        std::vector<std::pair<int, int>> retRoute;

        retRoute.push_back(pStart);
        double dSteep =
                (pEnd.first - pStart.first) != 0 ? (pEnd.second - pStart.second + 0.0) /
                                                   (pEnd.first - pStart.first)
                                                 : 0;
        double dHeight = pStart.second - dSteep * pStart.first;
        int start = pStart.first;
        int end = pEnd.first;
        int step = start > end ? -1 : 1;

        for (int j = start; (step > 0) == (j < end); j += step) {
            int yPre = std::round(dSteep * j + dHeight);
            int yPost = std::round(dSteep * (j + 1) + dHeight);
            int mMax = std::max(yPre, yPost);
            int mMin = std::min(yPre, yPost);
            std::vector<std::pair<int, int>> eachLine;
            for (int k = mMin; k <= mMax; k++) {
                if (gGrid->isInside(j, k) && (!hasMask || gMask->get(j, k))) {
                    gGrid->set(j, k, endValue);
                    eachLine.emplace_back(j, k);
                }
            }
            if (pStart.second < pEnd.second)
                retRoute.insert(retRoute.end(), eachLine.begin(), eachLine.end());
            else
                retRoute.insert(retRoute.end(), eachLine.rbegin(), eachLine.rend());

        }
        retRoute.push_back(pEnd);

        gGrid->set(pStart, endValue);
        gGrid->set(pEnd, endValue);

        return retRoute;
    }

    template<typename T>
    static void randToCenter(std::shared_ptr<gIGrid<T>> gGrid,
                             std::pair<int, int> pCenter, T endValue, std::vector<T> toStopValues,
                             std::shared_ptr<gIGrid<bool>> gMask = nullptr, int seed = -1) {
        hasMask = gMask != nullptr;
        if (seed == -1) {
            seed = std::chrono::system_clock::now().time_since_epoch().count();
        }
        std::set<T> sStopValues(toStopValues.begin(), toStopValues.end());

        auto gRange = gGrid->rangeUse();
        int GWidth = (gRange.first.second - gRange.first.first) + 1;
        int GHeight = (gRange.second.second - gRange.second.first) + 1;

        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> distrib(-GWidth - GHeight, GWidth + GHeight);
        int n = distrib(gen);

        //TODO REFACTOR THIS
        std::pair<int, int> pStart;
        if (n > 0)
            pStart = {std::min(n, GWidth), (n - GHeight > 0 ? n - GHeight : 0)};
        else
            pStart = {std::abs(n) - GWidth < 0 ? std::min(std::abs(n), GWidth) : 0,
                      (GHeight - (std::abs(n) - GWidth > 0 ? std::abs(n) - GHeight : 0))};


        double dSteep =
                (pCenter.first - pStart.first) != 0 ? (pCenter.second - pStart.second + 0.0) /
                                                      (pCenter.first - pStart.first)
                                                    : 0;
        double dHeight = pStart.second - dSteep * pStart.first;
        int start = pStart.first;
        int end = pCenter.first;
        int step = start > end ? -1 : 1;
        bool hasFoundRoad = false;

        for (int j = start; (step > 0) == (j < end); j += step) {
            int yPre = std::round(dSteep * j + dHeight);
            int yPost = std::round(dSteep * (j + 1) + dHeight);
            int mMax = std::max(yPre, yPost);
            int mMin = std::min(yPre, yPost);

            for (int k = mMin; k <= mMax; k++) {
                if (gGrid->isInside(j, k) && (sStopValues.find(gGrid->get(j, k)) != sStopValues.end())) {
                    gGrid->set(j, k, endValue);
                    hasFoundRoad = true;
                }
                if (gGrid->isInside(j, k) && (!hasMask || gMask->get(j, k)))
                    gGrid->set(j, k, endValue);
            }
            if (hasFoundRoad)
                return;
        }
    }
};

bool gBaseToLineRoads::hasMask;

#endif //CITYOFWEIRDFISHES_GBASETOLINEROADS_H

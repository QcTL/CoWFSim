//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOGRADIENTMINIMUM_H
#define CITYOFWEIRDFISHES_GBASETOGRADIENTMINIMUM_H

#include <utility>
#include <list>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>
#include "../gIGrid.h"

struct gtmElement {
    double cGoingDownBase;
    double cGoingUpBase;
    double cAddDownForIter;
    double cAddUpForIter;

    gtmElement(double cGoingDownBase, double cGoingUpBase, double cAddDownForIter, double cAddUpForIter)
            : cGoingDownBase(cGoingDownBase), cGoingUpBase(cGoingUpBase), cAddDownForIter(cAddDownForIter),
              cAddUpForIter(cAddUpForIter) {};
};

template<typename T>
class gBaseToGradientMinimum {
public:

    gBaseToGradientMinimum(const std::vector<gtmElement> &lElements,
                           std::pair<int, int> strBiggestPos,
                           std::shared_ptr<gIGrid<T>> gResult) : gtmGResult(std::move(gResult)),
                                                                 gtmLElements(lElements) {
        gtmToExtend.emplace_back(strBiggestPos, 0);
        gtmGResult->set(strBiggestPos.first, strBiggestPos.second, float(lElements.size() - 1));

        std::random_device rd;
        gen = std::mt19937(rd());
    }

    gBaseToGradientMinimum(const std::vector<gtmElement> &lElements,
                           std::pair<int, int> strBiggestPos,
                           std::shared_ptr<gIGrid<T>> gResult, int rSeed) : gtmGResult(std::move(gResult)),
                                                                            gtmLElements(lElements) {
        gtmToExtend.emplace_back(strBiggestPos, 0);
        gtmGResult->set(strBiggestPos.first, strBiggestPos.second, float(lElements.size() - 1));
        gen.seed(rSeed);
    }

    void generate() {
        while (!gtmToExtend.empty()) {
            std::pair<int, int> newPos = gtmToExtend.front().first;
            extendValueGrid(newPos, gtmGResult->get(newPos.first, newPos.second), gtmToExtend.front().second);
            gtmToExtend.pop_front();
        }
    }

private:
    std::shared_ptr<gIGrid<T>> gtmGResult;
    std::vector<gtmElement> gtmLElements;

    std::list<std::pair<std::pair<int, int>, int>> gtmToExtend;

    std::mt19937 gen;
    std::bernoulli_distribution d;

    void extendValueGrid(std::pair<int, int> pAct, float vChoose, int nConcurrent) {
        std::vector<std::pair<int, int>> dOffsets =
                {{pAct.first + 1, pAct.second},
                 {pAct.first - 1, pAct.second},
                 {pAct.first,     pAct.second + 1},
                 {pAct.first,     pAct.second - 1}};

        for (auto &dOffset: dOffsets) {
            if (gtmGResult->isInside(dOffset) && gtmGResult->get(dOffset.first, dOffset.second) == -1) {

                gtmElement gtmE = gtmLElements[static_cast<int>(vChoose)];
                float nValue = randomChoice(gtmE.cGoingDownBase + gtmE.cAddDownForIter * nConcurrent) ? vChoose - 1 :
                               randomChoice(gtmE.cGoingUpBase + gtmE.cAddUpForIter * nConcurrent) ? vChoose + 1 :
                               vChoose;
                nValue = std::clamp(nValue, 0.0f, static_cast<float>(gtmLElements.size() - 1));

                gtmGResult->set(dOffset.first, dOffset.second, nValue);
                gtmToExtend.emplace_back(dOffset, nValue == vChoose ? nConcurrent + 1 : 0);
            }
        }
    }

    bool randomChoice(double p) {
        d = std::bernoulli_distribution(p);
        return d(gen);
    }
};

#endif //CITYOFWEIRDFISHES_GBASETOGRADIENTMINIMUM_H

//
// Created by Laminar on 27/01/2024.
//

#include "gBaseToGradientMinimum.h"
#include "../gBasicGrid.h"

#include <utility>
#include <vector>
#include <random>
#include <algorithm>

gBaseToGradientMinimum::gBaseToGradientMinimum(const std::vector<gtmElement> &lElements,
                                               std::pair<int, int> strBiggestPos,
                                               std::unique_ptr<gIGrid<float>> gResult) : gtmGResult(std::move(gResult)),
                                                                                         gtmLElements(lElements) {
    gtmToExtend.emplace_back(strBiggestPos, 0);
    gtmGResult->set(strBiggestPos.first, strBiggestPos.second, float(lElements.size() - 1));

    std::random_device rd;
    gen = std::mt19937(rd());
}


gBaseToGradientMinimum::gBaseToGradientMinimum(const std::vector<gtmElement> &lElements,
                                               std::pair<int, int> strBiggestPos,
                                               std::unique_ptr<gIGrid<float>> gResult,
                                               int rSeed) : gtmGResult(std::move(gResult)),
                                                            gtmLElements(lElements) {
    gtmToExtend.emplace_back(strBiggestPos, 0);
    gtmGResult->set(strBiggestPos.first, strBiggestPos.second, float(lElements.size() - 1));
    gen.seed(rSeed);
}


bool gBaseToGradientMinimum::randomChoice(double p) {
    d = std::bernoulli_distribution(p);
    return d(gen);
}


void gBaseToGradientMinimum::extendValueGrid(std::pair<int, int> pAct, float vChoose, int nConcurrent) {
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

void gBaseToGradientMinimum::generate() {
    while (!gtmToExtend.empty()) {
        std::pair<int, int> newPos = gtmToExtend.front().first;
        extendValueGrid(newPos, gtmGResult->get(newPos.first, newPos.second), gtmToExtend.front().second);
        gtmToExtend.pop_front();
    }

    std::cout << *dynamic_cast<gBasicGrid<float> *>(gtmGResult.get()) << std::endl;
    std::cout << "------------" << std::endl;
}

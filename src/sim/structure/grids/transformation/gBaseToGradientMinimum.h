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
                           std::shared_ptr<gIGrid<T>> gResult,
                           const std::shared_ptr<gIGrid<bool>> &pMask = nullptr,
                           int rSeed = -1) : gtmGResult(std::move(gResult)),
                                             gtmLElements(lElements),
                                             hasMask(pMask != nullptr),
                                             gMask(pMask){
        gtmToExtend.emplace_back(strBiggestPos, 0);
        gtmGResult->set(strBiggestPos.first, strBiggestPos.second, float(lElements.size() - 1));

        if (rSeed == -1) {
            std::random_device rd;
            gen = std::mt19937(rd());
        } else {
            gen.seed(rSeed);
        }
    }


    void generateV1() {
        while (!gtmToExtend.empty()) {
            std::pair<int, int> newPos = gtmToExtend.front().first;
            extendValueGridV1(newPos, gtmGResult->get(newPos.first, newPos.second), gtmToExtend.front().second);
            gtmToExtend.pop_front();
        }
    }

    void generateV2() {
        while (!gtmToExtend.empty()) {
            std::pair<int, int> newPos = gtmToExtend.front().first;
            std::vector<std::pair<int, int>> dOffsets =
                    {{newPos.first,     newPos.second - 1},
                     {newPos.first + 1, newPos.second - 1},
                     {newPos.first + 1, newPos.second},
                     {newPos.first + 1, newPos.second + 1},
                     {newPos.first,     newPos.second + 1},
                     {newPos.first - 1, newPos.second + 1},
                     {newPos.first - 1, newPos.second},
                     {newPos.first - 1, newPos.second - 1}};
            for(int i = 0; i < dOffsets.size(); i++) {
                extendValueGridV2(dOffsets[i], 2, i, 0);
                gtmGResult->set(dOffsets[i].first, dOffsets[i].second, 2);
            }
            gtmToExtend.pop_front();
        }
    }

private:
    std::shared_ptr<gIGrid<T>> gtmGResult;
    std::vector<gtmElement> gtmLElements;


    bool hasMask;
    std::shared_ptr<gIGrid<bool>> gMask;

    std::list<std::pair<std::pair<int, int>, int>> gtmToExtend;

    std::mt19937 gen;
    std::bernoulli_distribution d;

    void extendValueGridV1(std::pair<int, int> pAct, float vChoose, int nConcurrent) {
        std::vector<std::pair<int, int>> dOffsets =
                {{pAct.first + 1, pAct.second},
                 {pAct.first - 1, pAct.second},
                 {pAct.first,     pAct.second + 1},
                 {pAct.first,     pAct.second - 1}};

        for (auto &dOffset: dOffsets) {
            if (gtmGResult->isInside(dOffset) && (!hasMask || gMask->get(dOffset)) && gtmGResult->get(dOffset.first, dOffset.second) == -1) {

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

    void extendValueGridV2(std::pair<int, int> pAct, float vChoose, uint8_t vDir, float nConcurrent) {
        std::vector<std::vector<std::pair<int, int>>> dOffsets =
                {{{pAct.first + 1, pAct.second - 1}, {pAct.first,     pAct.second - 1}},
                 {{pAct.first + 1, pAct.second - 1}, {pAct.first + 1, pAct.second}},
                 {{pAct.first + 1, pAct.second + 1}, {pAct.first + 1, pAct.second}},
                 {{pAct.first + 1, pAct.second + 1}, {pAct.first,     pAct.second + 1}},
                 {{pAct.first - 1, pAct.second + 1}, {pAct.first,     pAct.second + 1}},
                 {{pAct.first - 1, pAct.second + 1}, {pAct.first - 1, pAct.second}},
                 {{pAct.first - 1, pAct.second - 1}, {pAct.first - 1, pAct.second}},
                 {{pAct.first - 1, pAct.second - 1}, {pAct.first,     pAct.second - 1}}};

        for (int i = 0; i < dOffsets[vDir].size(); i++ ) {
            if (gtmGResult->isInside(dOffsets[vDir][i]) && (!hasMask || gMask->get(dOffsets[vDir][i])) && gtmGResult->get(dOffsets[vDir][i]) == -1) {

                gtmElement gtmE = gtmLElements[static_cast<int>(vChoose)];
                float nValue = randomChoice(gtmE.cGoingDownBase + gtmE.cAddDownForIter * nConcurrent) ? vChoose - 1 :
                               randomChoice(gtmE.cGoingUpBase + gtmE.cAddUpForIter * nConcurrent) ? vChoose + 1 :
                               vChoose;
                nValue = std::clamp(nValue, 0.0f, static_cast<float>(gtmLElements.size() - 1));

                gtmGResult->set(dOffsets[vDir][i], nValue);
                extendValueGridV2(dOffsets[vDir][i], gtmGResult->get(dOffsets[vDir][i]), vDir,
                                  nValue == vChoose ? nConcurrent + 2 + (1-i)*0.4 : 0);
            }
        }
    }

    bool randomChoice(double p) {
        d = std::bernoulli_distribution(p);
        return d(gen);
    }
};

#endif //CITYOFWEIRDFISHES_GBASETOGRADIENTMINIMUM_H

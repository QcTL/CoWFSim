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
#include "../gIGrid.h"


class gBaseToGradientMinimum {
public:
    struct gtmElement {
        double cGoingDownBase;
        double cGoingUpBase;
        double cAddDownForIter;
        double cAddUpForIter;

        gtmElement(double cGoingDownBase, double cGoingUpBase, double cAddDownForIter, double cAddUpForIter)
                : cGoingDownBase(cGoingDownBase), cGoingUpBase(cGoingUpBase), cAddDownForIter(cAddDownForIter),
                  cAddUpForIter(cAddUpForIter) {};
    };

    gBaseToGradientMinimum(const std::vector<gtmElement> &lElements, std::pair<int, int> strBiggestPos,
                           std::shared_ptr<gIGrid<float>> gResult);

    gBaseToGradientMinimum(const std::vector<gtmElement> &lElements, std::pair<int, int> strBiggestPos,
                           std::shared_ptr<gIGrid<float>> gResult, int rSeed);

    void generate();

private:
    std::shared_ptr<gIGrid<float>> gtmGResult;
    std::vector<gtmElement> gtmLElements;

    std::list<std::pair<std::pair<int, int>, int>> gtmToExtend;

    std::mt19937 gen;
    std::bernoulli_distribution d;

    void extendValueGrid(std::pair<int, int> pAct, float vChoose, int nConcurrent);

    bool randomChoice(double p);
};


#endif //CITYOFWEIRDFISHES_GBASETOGRADIENTMINIMUM_H

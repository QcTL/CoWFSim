//
// Created by Laminar on 16/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETORIVER_H
#define CITYOFWEIRDFISHES_GBASETORIVER_H

#include <memory>
#include <vector>
#include <valarray>
#include <list>
#include <random>
#include "../gIGrid.h"

template<typename T>
class gBaseToRiver {
private:

    struct oLine {
        std::pair<uint32_t, uint32_t> lP1;
        std::pair<uint32_t, uint32_t> lP2;
        std::vector<double> lF;

        oLine(const std::pair<uint32_t, uint32_t> &p1, const std::pair<uint32_t, uint32_t> &p2) {
            lP1 = (p1.first < p2.first) ? p1 : p2;
            lP2 = (p1.first < p2.first) ? p2 : p1;

            lF = line_equation(lP1, lP2);
            lWeight = std::sqrt(std::pow(lP2.first - lP1.first, 2) + std::pow(lP2.second - lP1.second, 2));
        }

        double getWeight() const {
            return lWeight;
        }

        double getSize() {
            return std::pow(lWeight, 2);
        }

        double getMaxDist(const std::pair<uint32_t, uint32_t> &p) {
            return std::max( (p.first - lP1.first) * (p.first - lP1.first) + (p.second - lP1.second) * (p.second - lP1.second),
                             (p.first - lP2.first) * (p.first - lP2.first) + (p.second - lP2.second) * (p.second - lP2.second)) ;
        }

    private:
        double lWeight;

        std::vector<double>
        line_equation(const std::pair<uint32_t, uint32_t> point1, const std::pair<uint32_t, uint32_t> point2) {
            double m = static_cast<double>(point2.second - point1.second) / (point2.first - point1.first);
            return {m, -1, point1.second - m * point1.first};
        }
    };


    std::pair<uint32_t, uint32_t>
    interpolate(const std::pair<uint32_t, uint32_t> &p1, const std::pair<uint32_t, uint32_t> &p2, double t) {
        return {(1 - t) * p1.first + t * p2.first, (1 - t) * p1.second + t * p2.second};
    }

    bool isInLine(std::pair<uint32_t, uint32_t> p, oLine l) {
        return l.getMaxDist(p) < l.getSize()+1;
    }

    double pointLineDist(const std::pair<uint32_t, uint32_t> &p, std::vector<double> rRect) {
        return std::abs(rRect[0] * p.first + rRect[1] * p.second + rRect[2]) /
               std::sqrt(rRect[0] * rRect[0] + rRect[1] * rRect[1]);
    }

public:
    explicit gBaseToRiver(std::shared_ptr<gIGrid<T>> gR, int rSeed = -1) {
        std::list<oLine> lLines;
        std::default_random_engine generator;
        if (rSeed != -1)
            generator.seed(rSeed);

        std::pair<uint32_t, uint32_t> p1 = {5, 5};
        //std::pair<uint32_t, uint32_t> p2 = {20, 73};
        std::pair<uint32_t, uint32_t> p3 = {100, 100};
        lLines = {oLine(p1, p3)};

        for (int i = 0; i < 20; i++) {
            std::vector<double> weights;
            for (const oLine &line: lLines) {
                weights.push_back(line.getWeight());
            }

            std::discrete_distribution<> distribution(weights.begin(), weights.end());

            int index = distribution(generator);
            typename std::list<oLine>::iterator nLine = lLines.begin();
            std::advance(nLine, index);
            std::pair<uint32_t, uint32_t> nPoint = interpolate(nLine->lP1, nLine->lP2, 0.5);
            std::vector<double> nPerp = {nLine->lF[1], -nLine->lF[0],
                                         -(nLine->lF[1] * nPoint.first - nLine->lF[0] * nPoint.second)};
            std::uniform_real_distribution<double> distFloatOffset(-11.0 / (i + 1), 11.0 / (i + 1));
            uint32_t nDisplacedX = nPoint.first + distFloatOffset(generator);
            std::pair<uint32_t, uint32_t> nPointEnd = {nDisplacedX, (nPerp[0] * nDisplacedX + nPerp[2]) / -nPerp[1]};

            lLines.insert(nLine, oLine(nLine->lP1, nPointEnd));
            lLines.insert(nLine, oLine(nPointEnd, nLine->lP2));
            //Final;
            lLines.erase(nLine);
        }

        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gR->rangeUse();
        double rWidth = 3;
        for (int i = gRange.first.first; i <= gRange.first.second; i++) {
            for (int j = gRange.second.first; j <= gRange.second.second; j++) {
                for (const oLine &lines: lLines) {
                    if (isInLine({j, i}, lines) && pointLineDist({j, i}, lines.lF) <= rWidth) {
                        gR->set(i, j, 1);
                    }
                }
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_GBASETORIVER_H

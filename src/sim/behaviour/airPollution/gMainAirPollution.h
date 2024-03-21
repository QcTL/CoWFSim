//
// Created by Laminar on 06/03/2024.
//

#ifndef CITYOFWEIRDFISHES_GMAINAIRPOLLUTION_H
#define CITYOFWEIRDFISHES_GMAINAIRPOLLUTION_H

#include <memory>
#include <valarray>
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "../../structure/grids/transformation/gBasicTransformations.h"

class gMainAirPollution {
public:
    explicit gMainAirPollution(int lSize) {
        gLayerAirPollution = std::make_shared<gBasicGrid<uint8_t >>
                (gBasicGrid<uint8_t>(lSize, lSize, 0));
        recomputeWindMat({-0.75, 0.5});
    }

    void tickReduced(std::shared_ptr<gIGrid<uint8_t>> &gBasicType) {

        BasicTransformations::copyWhere(gLayerAirPollution, gBasicType, {{3, 8}});


        BasicTransformations::Kernel<double> k(
                {windMat[0][0], windMat[0][1], windMat[0][2],
                 windMat[1][0], windMat[1][1], windMat[1][2],
                 windMat[2][0], windMat[2][1], windMat[2][2]}, 3, 3);


        auto range = gLayerAirPollution->rangeUse();
        int width = range.first.second - range.first.first + 1;
        std::vector<double> nAttributes(width * (range.second.second - range.second.first + 1), 0);

        std::vector<std::pair<int, int>> kernelIndices;
        std::vector<double> kernelValues;
        for (int y = 0; y < k.sY; y++) {
            for (int x = 0; x < k.sX; x++) {
                kernelIndices.push_back({x - k.sX / 2, y - k.sY / 2});
                kernelValues.push_back(k.dValues[y * k.sX + x]);
            }
        }

        int idx = 0;
        for (int i = range.second.first; i <= range.second.second; i++) {
            for (int j = range.first.first; j <= range.first.second; j++, idx++) {
                double r = 0;
                for (int k = 0; k < kernelIndices.size(); k++) {
                    int dx = i + kernelIndices[k].first;
                    int dy = j + kernelIndices[k].second;
                    if (gLayerAirPollution->isInside(dx, dy)) {
                        r += kernelValues[k] * (double) gLayerAirPollution->get(dx, dy) / 8.0 * 0.8;
                    }
                }
                nAttributes[idx] = r;
            }
        }

        idx = 0;
        for (int i = range.second.first; i <= range.second.second; i++) {
            for (int j = range.first.first; j <= range.first.second; j++, idx++) {
                gLayerAirPollution->set(i, j, static_cast<uint8_t>(
                        std::round(std::min(std::max(nAttributes[idx] - 0.005, 0.0), 1.0) * 8)));
            }
        }
    }

    std::shared_ptr<gIGrid<uint8_t>> gLayerAirPollution;
private:


    void recomputeWindMat(std::pair<double, double> newWind) {
        double norm = std::sqrt(newWind.first * newWind.first + newWind.second * newWind.second);
        dUnitWind = {newWind.first / norm, newWind.second / norm};

        std::vector<double> const_vector = {dUnitWind.first, dUnitWind.second};
        double totalSum = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                windMat[i][j] = std::max(0.0, bMat[i][j].first * const_vector[0] + bMat[i][j].second * const_vector[1]);
                totalSum += std::max(0.0, bMat[i][j].first * const_vector[0] + bMat[i][j].second * const_vector[1]);
            }
        }
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                windMat[i][j] = windMat[i][j] / totalSum;
            }
        }
    }

    std::vector<std::vector<std::pair<int, int>>> bMat = {
            {{-1, 1},  {0, 1},  {1, 1}},
            {{-1, 0},  {0, 0},  {1, 0}},
            {{-1, -1}, {0, -1}, {1, -1}}
    };

    std::vector<std::vector<double>> windMat = {
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0}
    };

    std::pair<double, double> dUnitWind;
};

#endif //CITYOFWEIRDFISHES_GMAINAIRPOLLUTION_H

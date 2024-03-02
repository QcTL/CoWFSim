//
// Created by Laminar on 28/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASICTRANSFORMATIONS_H
#define CITYOFWEIRDFISHES_GBASICTRANSFORMATIONS_H

#include <memory>
#include <vector>
#include <unordered_set>
#include <map>

#include "../gIGrid.h"
#include "../gBasicGrid.h"

class BasicTransformations {

public:

    template<typename T>
    struct Kernel {
        std::vector<T> dValues;
        int sX;
        int sY;

        Kernel(const std::vector<T> &dValues, int sX, int sY) : dValues(dValues), sX(sX), sY(sY) {}
    };

    template<typename T>
    static void GenericKernelTransformation(std::shared_ptr<gIGrid<T>> grid, Kernel<T> kernel) {
        auto range = grid->rangeUse();
        int width = range.first.second - range.first.first + 1;
        std::vector<T> nAttributes(width * (range.second.second - range.second.first + 1), 0);

        std::vector<std::pair<int, int>> kernelIndices;
        std::vector<T> kernelValues;
        for (int y = 0; y < kernel.sY; y++) {
            for (int x = 0; x < kernel.sX; x++) {
                kernelIndices.push_back({x - kernel.sX / 2, y - kernel.sY / 2});
                kernelValues.push_back(kernel.dValues[y * kernel.sX + x]);
            }
        }

        int idx = 0;
        for (int i = range.second.first; i <= range.second.second; i++) {
            for (int j = range.first.first; j <= range.first.second; j++, idx++) {
                T r = 0;
                for (int k = 0; k < kernelIndices.size(); k++) {
                    int dx = i + kernelIndices[k].first;
                    int dy = j + kernelIndices[k].second;
                    if (grid->isInside(dx, dy)) {
                        r += kernelValues[k] * grid->get(dx, dy);
                    }
                }
                nAttributes[idx] = r;
            }
        }

        idx = 0;
        for (int i = range.second.first; i <= range.second.second; i++) {
            for (int j = range.first.first; j <= range.first.second; j++, idx++) {
                grid->set(i, j, nAttributes[idx]);
            }
        }
    }

    template<typename T>
    static void GaussianFilter(std::shared_ptr<gIGrid<T>> grid) {

        BasicTransformations::Kernel<double> k({0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625}, 3,
                                               3);
        BasicTransformations::GenericKernelTransformation<T>(grid, k);
    }

    template<typename T>
    static std::shared_ptr<gIGrid<bool>> genMaskFromGrid(std::shared_ptr<gIGrid<T>> grid, std::vector<T> inMask) {
        std::unordered_set<T> sMask(inMask.begin(), inMask.end());
        auto gRange = grid->rangeUse();
        int GWidth = (gRange.first.second - gRange.first.first) + 1;
        int GHeight = (gRange.second.second - gRange.second.first) + 1;
        std::shared_ptr<gIGrid<bool>> retG = std::make_shared<gBasicGrid<bool>>(
                gBasicGrid<bool>(GWidth, GHeight, false));

        for (int i = gRange.second.first; i <= gRange.second.second; i++) {
            for (int j = gRange.first.first; j <= gRange.first.second; j++) {
                if (sMask.find(grid->get(i, j)) != sMask.end()) {
                    retG->set(i, j, true);
                }
            }
        }
        return retG;
    }

    template<typename T>
    static void replaceValues(const std::shared_ptr<gIGrid<T>> &grid, const std::map<T, T> &repValues) {
        auto gRange = grid->rangeUse();
        for (int i = gRange.second.first; i <= gRange.second.second; i++) {
            for (int j = gRange.first.first; j <= gRange.first.second; j++) {
                if (repValues.find(grid->get(i, j)) != repValues.end()) {
                    grid->set(i, j, repValues.at(grid->get(i, j)));
                }
            }
        }
    }

    template<typename T, typename Q>
    static void copyWhere(const std::shared_ptr<gIGrid<T>> &gridExit,
                          const std::shared_ptr<gIGrid<Q>> &gridEnter,
                          const std::map<Q, T> &repValues) {
        auto gRange = gridEnter->rangeUse();
        for (int i = gRange.second.first; i <= gRange.second.second; i++) {
            for (int j = gRange.first.first; j <= gRange.first.second; j++) {
                if (repValues.find(gridEnter->get(i, j)) != repValues.end()) {
                    gridExit->set(i, j, repValues.at(gridEnter->get(i, j)));
                }
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_GBASICTRANSFORMATIONS_H

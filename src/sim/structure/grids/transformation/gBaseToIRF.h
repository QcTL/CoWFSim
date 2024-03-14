//
// Created by Laminar on 03/03/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOIRF_H
#define CITYOFWEIRDFISHES_GBASETOIRF_H

#include <utility>
#include <memory>
#include <valarray>
#include <random>
#include "../gIGrid.h"

class gBaseToIRF {

public:

    struct gPositionEscape {
        std::pair<double, double> pPerpOrigin;
        std::pair<double, double> pPerpStart;
    };

    template<typename T>
    static std::vector<gPositionEscape>
    gen(const std::shared_ptr<gIGrid<T>> gOutput, const std::shared_ptr<gIGrid<uint8_t>> &gType,
        std::pair<int, int> pStart, std::pair<double, double> vStart, T valueEnd, uint8_t valueTypeSoil,
        uint8_t fDispersion, int seed = -1) {

        std::vector<gPositionEscape> ret;
        std::pair<double, double> vUnitInertia = vStart;
        std::pair<int, int> pActive = pStart;

        double deviation = 4.0;
        int tToDisp = fDispersion;

        std::default_random_engine re(seed == -1 ? std::random_device{}() : seed);
        std::uniform_real_distribution<double> unif(-deviation, deviation);

        while (gType->isInside(pActive)) {
            if (gType->get(pActive) != 5 && getSurroundRoads(gType, pActive, valueTypeSoil) <= 2) {
                gOutput->set(pActive, valueEnd);
                gType->set(pActive, valueTypeSoil);
            }

            if (tToDisp <= 0) {
                std::pair<double, double> perUnitInertia = generateRandomPerpendicular(vUnitInertia);
                std::pair<double, double> perStart;
                if (std::abs(perUnitInertia.first) > std::abs(perUnitInertia.second)) {
                    perStart = {pActive.first + (1 * (perUnitInertia.first > 0 ? 1 : -1)), pActive.second};
                } else {
                    perStart = {pActive.first, pActive.second + (1 * (perUnitInertia.second > 0 ? 1 : -1))};
                }
                ret.push_back({{-perUnitInertia.second, perUnitInertia.first}, perStart});
                ret.push_back({{perUnitInertia.second, -perUnitInertia.first}, perStart});

                tToDisp = fDispersion;
            }

            uint8_t rDir = getRandomNumberProportionalToVector(vUnitInertia);
            if (rDir == 0 ? vUnitInertia.first > 0 : vUnitInertia.second > 0) {
                pActive.first = pActive.first + (rDir == 0 ? 1 : 0);
                pActive.second = pActive.second + (rDir == 0 ? 0 : 1);
            } else {
                pActive.first = pActive.first + (rDir == 0 ? -1 : 0);
                pActive.second = pActive.second + (rDir == 0 ? 0 : -1);
            }
            double angleDegrees = unif(re);


            double angleRadians = angleDegrees * M_PI / 180.0;
            vUnitInertia.first = vUnitInertia.first * cos(angleRadians) - vUnitInertia.second * sin(angleRadians);
            vUnitInertia.second = vUnitInertia.first * sin(angleRadians) + vUnitInertia.second * cos(angleRadians);

            tToDisp--;
        }

        return ret;
    }

private:
    static int getSurroundRoads(const std::shared_ptr<gIGrid<uint8_t>> &gType, const std::pair<int, int> gPosition,
                                const uint8_t cCommon) {
        std::vector<std::pair<int, int>> vNear = {{0,  1},
                                                  {0,  -1},
                                                  {1,  0},
                                                  {-1, 0}};
        uint8_t numNear = 0;
        for (const std::pair<int, int> &gPos: vNear) {
            if (gType->isInside(gPosition.first + gPos.first, gPosition.second + gPos.second) &&
                gType->get(gPosition.first + gPos.first, gPosition.second + gPos.second) == cCommon)
                numNear++;
        }
        return numNear;
    }

    static int getRandomNumberProportionalToVector(const std::pair<double, double> &vector) {
        double magnitude = std::sqrt(vector.first * vector.first + vector.second * vector.second);
        double probX = std::abs(vector.first) / magnitude;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        double randomValue = dis(gen);

        if (randomValue < probX) {
            return 0;
        } else {
            return 1;
        }
    }

    static std::pair<double, double> generateRandomPerpendicular(std::pair<double, double> dOrigin) {
        std::uniform_real_distribution<double> angle_dist(0.0, 2.0 * M_PI);
        std::default_random_engine re;
        double random_angle = angle_dist(re);

        double v_x = cos(random_angle);
        double v_y = sin(random_angle);

        double norm = sqrt(v_x * v_x + v_y * v_y);
        v_x /= norm;
        v_y /= norm;

        return std::make_pair(v_x, v_y);
    }

};

#endif //CITYOFWEIRDFISHES_GBASETOIRF_H

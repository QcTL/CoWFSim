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
    template<typename T>
    static void
    gen(const std::shared_ptr<gIGrid<T>> gOutput, const std::shared_ptr<gIGrid<uint8_t>> &gType,
        std::pair<int, int> pStart, std::pair<double, double> vStart,
        std::vector<T> valueEnd, uint8_t fDispersion, uint8_t maxDepthFractal, int seed = -1) {

        std::pair<double, double> vUnitInertia = vStart;
        std::pair<int, int> pActive = pStart;

        double deviation = 6.0;
        int tToDisp = fDispersion;

        std::default_random_engine re(seed == -1 ? std::random_device{}() : seed);
        std::uniform_real_distribution<double> unif(-deviation, deviation);

        while (gType->isInside(pActive) and gType->get(pActive) != 5) {
            gOutput->set(pActive, valueEnd[maxDepthFractal]);
            gType->set(pActive, 2);

            if (tToDisp <= 0 and maxDepthFractal > 0) {
                std::pair<double, double> perUnitInertia = generateRandomPerpendicular(vUnitInertia);
                std::pair<double, double> perStart;
                if (std::abs(perUnitInertia.first) > std::abs(perUnitInertia.second)) {
                    perStart = {pActive.first + (1 * (perUnitInertia.first > 0 ? 1 : -1)), pActive.second};
                } else {
                    perStart = {pActive.first, pActive.second + (1 * (perUnitInertia.second > 0 ? 1 : -1))};
                }
                gen(gOutput, gType, perStart, perUnitInertia, valueEnd, fDispersion, maxDepthFractal - 1);
                tToDisp = fDispersion * 1.5;
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
    }

private:
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

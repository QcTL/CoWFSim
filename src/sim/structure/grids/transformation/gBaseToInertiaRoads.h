//
// Created by Laminar on 03/03/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOINERTIAROADS_H
#define CITYOFWEIRDFISHES_GBASETOINERTIAROADS_H

#include <utility>
#include <memory>
#include <valarray>
#include <random>
#include "../gIGrid.h"

class gBaseToInertiaRoads {
public:
    template<typename T>
    static void
    gen(const std::shared_ptr<gIGrid<T>> gOutput, const std::shared_ptr<gIGrid<uint8_t>> &gType, std::pair<int, int> pStart,
        std::pair<double, double> vStart, T valueEnd, int seed = -1) {

        std::pair<double, double> vUnitInertia = vStart;
        std::pair<int, int> pActive = pStart;

        double deviation = 12.0;

        std::default_random_engine re(seed == -1 ? std::random_device{}() : seed);
        std::uniform_real_distribution<double> unif(-deviation, deviation);


        while (gType->get(pActive) == 0 and gType->isInside(pActive)) {
            gOutput->set(pActive, valueEnd);
            uint8_t rDir = getRandomNumberProportionalToVector(vUnitInertia);
            if (rDir == 0 ? vUnitInertia.first > 0 : vUnitInertia.second > 0) {
                pActive.first = pActive.first + (rDir == 0 ? 1 : 0);
                pActive.second = pActive.second + (rDir == 0 ? 0 : 1);
            } else {
                pActive.first = pActive.first + (rDir == 0 ? -1 : 0);
                pActive.second = pActive.second + (rDir == 0 ? 0 : -1);
            }
            double angleDegrees = unif(re);
            //GET RANDOM ANGLE DEGREES BETWEEN -3 and 3;

            double angleRadians = angleDegrees * M_PI / 180.0;
            vUnitInertia.first = vUnitInertia.first * cos(angleRadians) - vUnitInertia.second * sin(angleRadians);
            vUnitInertia.second = vUnitInertia.first * sin(angleRadians) + vUnitInertia.second * cos(angleRadians);
            std::cout<< vUnitInertia.first << " [] " << vUnitInertia.second << " for "<< angleDegrees <<  std::endl;
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
};

#endif //CITYOFWEIRDFISHES_GBASETOINERTIAROADS_H

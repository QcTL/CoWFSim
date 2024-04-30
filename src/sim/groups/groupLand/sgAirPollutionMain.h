//
// Created by Laminar on 06/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SGAIRPOLLUTIONMAIN_H
#define CITYOFWEIRDFISHES_SGAIRPOLLUTIONMAIN_H

#include <memory>
#include <valarray>
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "../../structure/grids/transformation/gBasicTransformations.h"

/**
 * @class sgAirPollutionMain
 * @brief Class that controls the layer of air pollution and its changes
 */
class sgAirPollutionMain : sEventReceiver {
public:
    explicit sgAirPollutionMain(int inGridSize) {
        std::shared_ptr<sEventManager> _sEM = sEventManager::getInstance();
        _sEM->addReceiverToEvent(sEventManager::sEM_Events::sEM_EventStartDay, this);


        gLayerAirPollution = std::make_shared<gBasicGrid<uint8_t >>
                (gBasicGrid<uint8_t>(inGridSize, inGridSize, 0));
        recomputeWindMat({-0.75, 0.5});

        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            sAPM_genRand.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            sAPM_genRand.seed(static_cast<unsigned int>(time(nullptr)));
    }

    /**
     * @brief The function that is called every 5 minutes in simulation
     * @param gBasicType The grid grid with the types of buildings presents
     */
    void tickReduced(std::shared_ptr<gIGrid<uint8_t>> &gBasicType) const {

        //Get the position of the heavy factories
        BasicTransformations::copyWhere(gLayerAirPollution, gBasicType, {{3, 8}});
    }


    /** @fn void er_EventStartDay
     *@brief Function that moves the progression of the wind one step, and have a possibility of doing a wind direction change.
     * @param inTDate The date of the new day
     */
    void er_EventStartDay(uint32_t inTDate) override {
        std::uniform_int_distribution<int> _distRandomChangeWind(0, 15);

        if (_distRandomChangeWind(sAPM_genRand) == 0) {
            std::uniform_real_distribution<double> _distNewWindDir(-1.0, 1.0);
            recomputeWindMat({_distNewWindDir(sAPM_genRand), _distNewWindDir(sAPM_genRand)});
        }

        BasicTransformations::Kernel<double> _k(
                {sAPM_windMat[0][0], sAPM_windMat[0][1], sAPM_windMat[0][2],
                 sAPM_windMat[1][0], sAPM_windMat[1][1], sAPM_windMat[1][2],
                 sAPM_windMat[2][0], sAPM_windMat[2][1], sAPM_windMat[2][2]}, 3, 3);

        auto range = gLayerAirPollution->rangeUse();
        int width = range.first.second - range.first.first + 1;
        std::vector<double> nAttributes(width * (range.second.second - range.second.first + 1), 0);

        std::vector<std::pair<int, int>> kernelIndices;
        std::vector<double> kernelValues;
        for (int y = 0; y < _k.sY; y++) {
            for (int x = 0; x < _k.sX; x++) {
                kernelIndices.push_back({x - _k.sX / 2, y - _k.sY / 2});
                kernelValues.push_back(_k.dValues[y * _k.sX + x]);
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

    /**
     * @fn uint32_t getPenalizationAir
     * @brief Given a position return the penalization of the building of being under the air pollution
     * @param inPCell The position of the cell
     * @return The penalization value.
     */
    uint32_t getPenalizationAir(const std::pair<int, int> inPCell) {
        return 5 * gLayerAirPollution->get(inPCell);
    }

    std::shared_ptr<gIGrid<uint8_t>> gLayerAirPollution;
private:

    /**
     * @fn void recomputeWindMat
     * @brief Given a wind direction return the kernel necessary to disperse the wind in that direction
     * @param newWind A pair of any value, it will be normalized
     */
    void recomputeWindMat(std::pair<double, double> newWind) {
        double _magVector = std::sqrt(newWind.first * newWind.first + newWind.second * newWind.second);
        std::pair<double, double> _dUnitWind = {newWind.first / _magVector, newWind.second / _magVector};

        std::vector<double> _unitVector = {_dUnitWind.first, _dUnitWind.second};
        double totalSum = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                sAPM_windMat[i][j] = std::max(0.0, sAPM_bMat[i][j].first * _unitVector[0] + sAPM_bMat[i][j].second * _unitVector[1]);
                totalSum += std::max(0.0, sAPM_bMat[i][j].first * _unitVector[0] + sAPM_bMat[i][j].second * _unitVector[1]);
            }
        }
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                sAPM_windMat[i][j] = sAPM_windMat[i][j] / totalSum;
            }
        }
    }

    std::vector<std::vector<std::pair<int, int>>> sAPM_bMat = {
            {{-1, 1},  {0, 1},  {1, 1}},
            {{-1, 0},  {0, 0},  {1, 0}},
            {{-1, -1}, {0, -1}, {1, -1}}
    };

    std::vector<std::vector<double>> sAPM_windMat = {
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0}
    };

    std::mt19937 sAPM_genRand;
};

#endif //CITYOFWEIRDFISHES_SGAIRPOLLUTIONMAIN_H

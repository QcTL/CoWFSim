//
// Created by Laminar on 13/03/2024.
//

#ifndef CITYOFWEIRDFISHES_RROADLINEVIEW_H
#define CITYOFWEIRDFISHES_RROADLINEVIEW_H

#include "rRoadViewMenu.h"

class rRoadLineView : public rRoadViewMenu {
public:
    explicit rRoadLineView(const std::shared_ptr<rIMenu> &mParent, const std::shared_ptr<rRNodeI> &refView,
                           rIMenu::rRelativePos rPos)
            : rRoadViewMenu(mParent, refView, "d_mRoadsViewLayer", rPos) {

        pElemCrossesNRoadsTop = {{},
                                 {}};
        pElemCrossesNRoadsBottom = {{},
                                    {}};
        pElemNumSize = {};

        gHeightsTop = {6, 4, 2};
        gHeightsBottom = {12, 10, 8};

        for (int i = 0; i < dExtracted.size(); ++i) {
            for (int j = 0; j < dExtracted[i].size(); ++j) {
                auto row = (rPos == pBottomLeft || rPos == pBottomRight) ? dExtracted.size() - 1 - i : i;
                auto col = (rPos == pTopRight || rPos == pBottomRight) ? dExtracted[i].size() - 1 - j : j;
                int value = dExtracted[i][j];
                if (value == 105) pElemOcc.emplace_back(row, col);
                else if (value == 277) {
                    auto &container = (i < 8) ? pElemCrossesNRoadsTop[j == 8] : pElemCrossesNRoadsBottom[j == 8];
                    container.emplace_back(row, col);
                } else if (value == 48) pElemNumSize.emplace_back(row, col);
            }
        }
    }

    void update() override {
        setSizeRoads(rSelRoad->getSizeRoad());
        setRoadsCars(rSelRoad->getPosRoad(0), rSelRoad->getPosRoad(1));
        setNewOcc(rSelRoad->getOccupancy());
        setNumberComp(rSelRoad->getCapacity());
    }

private:
    std::vector<std::pair<int, int>> pElemOcc;
    std::vector<std::pair<int, int>> pElemNumSize;

    std::vector<std::vector<std::pair<int, int>>> pElemCrossesNRoadsTop;
    std::vector<std::vector<std::pair<int, int>>> pElemCrossesNRoadsBottom;

    std::vector<uint8_t> gHeightsTop;
    std::vector<uint8_t> gHeightsBottom;

    void setNewOcc(const float fRate) {
        if (fRate < 0 || fRate > 1) return;

        int nTurnOn = static_cast<int>(5 * (1 - fRate));
        for (int i = 0; i < pElemOcc.size(); i++) {
            auto quad = &dInfo[(pElemOcc[i].second + pElemOcc[i].first * gWidth) * 4];
            auto tileIndex = (i >= nTurnOn) ? 105 : 32;
            for (int k = 0; k < 4; k++)
                quad[k].texCoords = lRefTiles[tileIndex][k];
        }
    }

    void setSizeRoads(const int capRoad) {
        int nToTake = 1 + 2 * (capRoad - 1);
        for (int i = 0; i < pElemCrossesNRoadsTop[0].size(); i++) {
            auto *quadTop0 = &dInfo[
                    (pElemCrossesNRoadsTop[0][i].second + pElemCrossesNRoadsTop[0][i].first * gWidth) * 4];
            auto *quadTop1 = &dInfo[
                    (pElemCrossesNRoadsTop[1][i].second + pElemCrossesNRoadsTop[1][i].first * gWidth) * 4];
            auto *quadBtm0 = &dInfo[
                    (pElemCrossesNRoadsBottom[0][i].second + pElemCrossesNRoadsBottom[0][i].first * gWidth) * 4];
            auto *quadBtm1 = &dInfo[
                    (pElemCrossesNRoadsBottom[1][i].second + pElemCrossesNRoadsBottom[1][i].first * gWidth) * 4];
            int texIndex = (i <= nToTake) ? 32 : 277;
            for (int k = 0; k < 4; k++) {
                quadTop0[k].texCoords = lRefTiles[texIndex][k];
                quadTop1[k].texCoords = lRefTiles[texIndex][k];
                quadBtm0[k].texCoords = lRefTiles[texIndex][k];
                quadBtm1[k].texCoords = lRefTiles[texIndex][k];
            }
            if (i <= nToTake) {
                for (int j = 3; j < 32; j++) {
                    sf::Vertex *quadTop = &dInfo[(gHeightsBottom[i] * gWidth + j) * 4];
                    sf::Vertex *quadBottom = &dInfo[(gHeightsTop[i] * gWidth + j) * 4];
                    for (int k = 0; k < 4; k++) {
                        quadTop[k].texCoords = lRefTiles[32][k];
                        quadBottom[k].texCoords = lRefTiles[32][k];
                    }
                }
            }
        }
    }

    void setNumberComp(const uint16_t rComp) {
        std::string vToShow = uint16_to_padded_string(rComp);
        for (int i = 0; i < 4; i++)
            for (int k = 0; k < 4; k++)
                dInfo[(pElemNumSize[i].second + pElemNumSize[i].first * gWidth) * 4 + k].texCoords
                        = lRefTiles[vToShow[i] - '0' + 48][k];
    }

    void setRoadsCars(const std::vector<std::list<uint32_t>> &rListPosTop,
                      const std::vector<std::list<uint32_t>> &rListPosBottom) {

        std::vector<std::vector<std::list<uint32_t>>> sPos = {rListPosTop, rListPosBottom};
        std::vector<std::vector<uint8_t>> sRefPos = {gHeightsBottom, gHeightsTop};

        for (int j = 0; j < 2; j++)
            for (int i = 0; i < sPos[j].size(); i++)
                for (const auto &p: sPos[j][i])
                    for (int k = 0; k < 4; k++)
                        dInfo[(sRefPos[j][i] * gWidth + 3 +
                               static_cast<int>((static_cast<float>(p) / static_cast<float>(rSelRoad->getCapacity())) *
                                                (32 - 3))) * 4 + k].texCoords = lRefTiles[48][k];
    }
};

#endif //CITYOFWEIRDFISHES_RROADLINEVIEW_H

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
                if (dExtracted[i][j] == 105) {
                    pElemOcc.emplace_back(row, col);
                } else if (dExtracted[i][j] == 277) {
                    if (i < 8)
                        pElemCrossesNRoadsTop[j == 8].emplace_back(row, col);
                    else
                        pElemCrossesNRoadsBottom[j == 8].emplace_back(row, col);
                } else if (dExtracted[i][j] == 48) {
                    pElemNumSize.emplace_back(row, col);
                }
            }
        }

        setNewOcc(refView->getOccupancy());
        setNumberComp(refView->getCapacity());

        setSizeRoads(refView->getSizeRoad(), true);
        setSizeRoads(refView->getSizeRoad(), false);

        setRoadsCars(refView->getPosRoad(0), refView->getPosRoad(1));

    }

    void update() override {

        setSizeRoads(rSelRoad->getSizeRoad(), true);
        setSizeRoads(rSelRoad->getSizeRoad(), false);
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
        if (fRate < 0 || fRate > 1)
            return;
        int nTurnOn = (int) (5 * (1 - fRate));
        for (int i = 0; i < pElemOcc.size(); i++) {
            sf::Vertex *quad = &dInfo[(pElemOcc[i].second + pElemOcc[i].first * gWidth) * 4];
            if (i >= nTurnOn) {
                for (int k = 0; k < 4; k++)
                    quad[k].texCoords = lRefTiles[105][k];
            } else {
                for (int k = 0; k < 4; k++)
                    quad[k].texCoords = lRefTiles[32][k];
            }
        }
    }

    void setSizeRoads(const int capRoad, const bool isTopRoad) {
        int nToTake = 1 + 2 * (capRoad - 1);
        if (isTopRoad) {
            for (int i = 0; i < pElemCrossesNRoadsTop[0].size(); i++) {
                sf::Vertex *quad0 = &dInfo[
                        (pElemCrossesNRoadsTop[0][i].second + pElemCrossesNRoadsTop[0][i].first * gWidth) * 4];
                sf::Vertex *quad1 = &dInfo[
                        (pElemCrossesNRoadsTop[1][i].second + pElemCrossesNRoadsTop[1][i].first * gWidth) * 4];

                int texIndex = (i <= nToTake) ? 32 : 277;
                for (int k = 0; k < 4; k++) {
                    quad0[k].texCoords = lRefTiles[texIndex][k];
                    quad1[k].texCoords = lRefTiles[texIndex][k];
                }
                if (i <= nToTake) {
                    for (int j = 3; j < 32; j++) {
                        sf::Vertex *quadTop = &dInfo[(gHeightsBottom[i] * gWidth + j) * 4];
                        for (int k = 0; k < 4; k++) {
                            quadTop[k].texCoords = lRefTiles[32][k];
                        }
                    }
                }
            }
        } else {
            for (int i = 0; i < pElemCrossesNRoadsBottom[0].size(); i++) {
                sf::Vertex *quad0 = &dInfo[
                        (pElemCrossesNRoadsBottom[0][i].second + pElemCrossesNRoadsBottom[0][i].first * gWidth) *
                        4];
                sf::Vertex *quad1 = &dInfo[
                        (pElemCrossesNRoadsBottom[1][i].second + pElemCrossesNRoadsBottom[1][i].first * gWidth) *
                        4];
                int texIndex = (i <= nToTake) ? 32 : 277;
                for (int k = 0; k < 4; k++) {
                    quad0[k].texCoords = lRefTiles[texIndex][k];
                    quad1[k].texCoords = lRefTiles[texIndex][k];
                }
                if (i <= nToTake) {
                    for (int j = 3; j < 32; j++) {
                        sf::Vertex *quadTop = &dInfo[(gHeightsTop[i] * gWidth + j) * 4];
                        for (int k = 0; k < 4; k++) {
                            quadTop[k].texCoords = lRefTiles[32][k];
                        }
                    }
                }
            }
        }
    }

    void setNumberComp(const uint16_t rComp) {
        std::string vToShow = uint16_to_padded_string(rComp);
        for (int i = 0; i < 4; i++) {
            sf::Vertex *quad = &dInfo[(pElemNumSize[i].second + pElemNumSize[i].first * gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                quad[k].texCoords = lRefTiles[vToShow[i] - '0' + 48][k];
            }
        }
    }

    void setRoadsCars(const std::vector<std::list<uint32_t>> &rListPosTop,
                      const std::vector<std::list<uint32_t>> &rListPosBottom) {
        //Check the position relative and put it in the corresponding position;
        for (int i = 0; i < rListPosTop.size(); i++) {
            for (const auto &p: rListPosTop[i]) {
                sf::Vertex *quad = &dInfo[
                        (gHeightsBottom[i] * gWidth + 3 +
                         (int) (((float) p / (float) rSelRoad->getCapacity()) * (32 - 3))) * 4];
                for (int k = 0; k < 4; k++) {
                    quad[k].texCoords = lRefTiles[48][k];
                }
            }
        }
        for (int i = 0; i < rListPosBottom.size(); i++) {
            for (const auto &p: rListPosBottom[i]) {
                sf::Vertex *quad = &dInfo[
                        (gHeightsTop[i] * gWidth + 3 +
                         (int) (((float) p / (float) rSelRoad->getCapacity()) * (32 - 3))) * 4];
                for (int k = 0; k < 4; k++) {
                    quad[k].texCoords = lRefTiles[48][k];
                }
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_RROADLINEVIEW_H

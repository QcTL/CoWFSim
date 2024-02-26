//
// Created by ganymede on 2/24/24.
//

#ifndef CITYOFWEIRDFISHES_RROADVIEWMENU_H
#define CITYOFWEIRDFISHES_RROADVIEWMENU_H

#include "../rIMenu.h"
#include "../../../sim/roads/node/rRNode.h"

std::string uint16_to_padded_string(const uint16_t num) {
    std::string str = std::to_string(num);
    if (str.length() > 4) {
        str = str.substr(0, 4);
    } else {
        while (str.length() < 4) {
            str.insert(0, "0");
        }
    }
    return str;
}

class rRoadViewMenu : public rIMenu {
public:
    explicit rRoadViewMenu(const std::shared_ptr<rIMenu> &mParent, const std::shared_ptr<rRNodeI> &refView,
                           const std::string &pthFileD, rIMenu::rRelativePos rPos)
            : rIMenu(mParent, rPos), rSelRoad(refView) {

        std::vector<std::vector<int>> data = extractDataFromFile(pthFileD);

        pElemNRoadsTop = {{},
                          {}};
        pElemNRoadsBottom = {{},
                             {}};
        pElemNumSize = {};

        for (int i = 0; i < data.size(); ++i) {
            for (int j = 0; j < data[i].size(); ++j) {
                auto row = (rPos == pBottomLeft || rPos == pBottomRight) ? data.size() - 1 - i : i;
                auto col = (rPos == pTopRight || rPos == pBottomRight) ? data[i].size() - 1 - j : j;
                if (data[i][j] == 105) {
                    pElemOcc.emplace_back(row, col);
                } else if (data[i][j] == 277) {
                    if (i < 8)
                        pElemNRoadsTop[j == 8].emplace_back(row, col);
                    else
                        pElemNRoadsBottom[j == 8].emplace_back(row, col);
                } else if (data[i][j] == 48) {
                    pElemNumSize.emplace_back(row, col);
                }
            }
        }

        dInfo = getVertexMenu((int) data[0].size(), (int) data.size(), data);
        gWidth = (int) data[0].size();
        gHeight = (int) data.size();

        setNewOcc(refView->getOccupancy());
        setNumberComp(refView->getCapacity());

        int gDiff = 30 / (refView->getCapacity() + 1);
        for (int i = 0; i < refView->getCapacity(); i++) {
            pPosCarTop.emplace_back(2, (i + 1) * gDiff);
            pPosCarBottom.emplace_back(8, (i + 1) * gDiff);
        }


        setSizeRoads(1, true);
        setSizeRoads(1, false);

        setRoadsCars(refView->getPosRoad(0), refView->getPosRoad(1));
    }

    void update() override {
        setRoadsCars(rSelRoad->getPosRoad(0), rSelRoad->getPosRoad(1));
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(dInfo, &tsTex.tsTex);
    }

    void setResponse(int v) override {}

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    parentMenu->setResponse(0);
                }
                break;
            default:
                break;
        }
        return false;
        //FALTA QUE ES PUGI ACTUALIZAR CADA TICK PER PODER REPRESENTAR EL DALLO;
    }

    void pressedCell(std::pair<int, int> cPressed) {
        parentMenu->setResponse(0);
    }

private:

    std::vector<std::pair<int, int>> pElemOcc;
    std::vector<std::pair<int, int>> pElemNumSize;

    std::vector<std::vector<std::pair<int, int>>> pElemNRoadsTop;
    std::vector<std::pair<int, int>> pPosCarTop;

    std::vector<std::vector<std::pair<int, int>>> pElemNRoadsBottom;
    std::vector<std::pair<int, int>> pPosCarBottom;

    sf::VertexArray dInfo;
    int gWidth = 0;
    int gHeight = 0;

    std::shared_ptr<rRNodeI> rSelRoad;

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
            for (int i = 0; i < pElemNRoadsTop[0].size(); i++) {
                sf::Vertex *quad0 = &dInfo[(pElemNRoadsTop[0][i].second + pElemNRoadsTop[0][i].first * gWidth) * 4];
                sf::Vertex *quad1 = &dInfo[(pElemNRoadsTop[1][i].second + pElemNRoadsTop[1][i].first * gWidth) * 4];

                int texIndex = (i <= nToTake) ? 32 : 277;
                for (int k = 0; k < 4; k++) {
                    quad0[k].texCoords = lRefTiles[texIndex][k];
                    quad1[k].texCoords = lRefTiles[texIndex][k];
                }
            }
        } else {
            for (int i = 0; i < pElemNRoadsBottom[0].size(); i++) {
                sf::Vertex *quad0 = &dInfo[(pElemNRoadsBottom[0][i].second + pElemNRoadsBottom[0][i].first * gWidth) *
                                           4];
                sf::Vertex *quad1 = &dInfo[(pElemNRoadsBottom[1][i].second + pElemNRoadsBottom[1][i].first * gWidth) *
                                           4];
                int texIndex = (i <= nToTake) ? 32 : 277;
                for (int k = 0; k < 4; k++) {
                    quad0[k].texCoords = lRefTiles[texIndex][k];
                    quad1[k].texCoords = lRefTiles[texIndex][k];
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

    void setRoadsCars(const std::list<uint32_t> &rListPosTop, const std::list<uint32_t> &rListPosBottom) {
        //1rst clear all the lines that could be had cars from the last one;
        for (int i = 3; i < 33; i++) {
            sf::Vertex *quadTop = &dInfo[(2 * gWidth + i) * 4];
            sf::Vertex *quadBottom = &dInfo[(8 * gWidth + i) * 4];
            for (int k = 0; k < 4; k++) {
                quadTop[k].texCoords = lRefTiles[32][k];
                quadBottom[k].texCoords = lRefTiles[32][k];
            }
        }

        //Check the position relative and put it in the corresponding position;
        for (const auto &p: rListPosTop) {
            sf::Vertex *quad = &dInfo[(pPosCarTop[p].second + pPosCarTop[p].first * gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                quad[k].texCoords = lRefTiles[48][k];
            }
        }
        for (const auto &p: rListPosBottom) {
            sf::Vertex *quad = &dInfo[(pPosCarBottom[p].second + pPosCarBottom[p].first * gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                quad[k].texCoords = lRefTiles[48][k];
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_RROADVIEWMENU_H

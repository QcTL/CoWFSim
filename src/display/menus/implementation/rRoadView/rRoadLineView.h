//
// Created by Laminar on 13/03/2024.
//

#ifndef CITYOFWEIRDFISHES_RROADLINEVIEW_H
#define CITYOFWEIRDFISHES_RROADLINEVIEW_H

#include "rRoadViewMenu.h"
#include "rRoadPresentView.h"

class rRoadLineView : public rRoadViewMenu {
public:
    explicit rRoadLineView(const std::shared_ptr<rIMenu> &mParent, const std::shared_ptr<rRNodeI> &refView, const std::shared_ptr<rPileMenus> &mPiles)
            : rRoadViewMenu(mParent, refView, "d_mRoadsViewLayer"), rRLV_rPileMenus(mPiles),
              pElemCrossNRoadsTop(2), pElemCrossNRoadsBottom(2) {

        pElemNumSize = {};

        gHeightsTop = {6, 4, 2};
        gHeightsBottom = {12, 10, 8};

        for (int i = 0; i < rIM_dExtracted.size(); ++i) {
            for (int j = 0; j < rIM_dExtracted[i].size(); ++j) {
                auto row = (rIM_rPos == pBottomLeft || rIM_rPos == pBottomRight) ? rIM_dExtracted.size() - 1 - i : i;
                auto col = (rIM_rPos == pTopRight || rIM_rPos == pBottomRight) ? rIM_dExtracted[i].size() - 1 - j : j;
                int value = rIM_dExtracted[i][j];
                if (value == 105) pElemOcc.emplace_back(row, col);
                else if (value == 277) {
                    auto &container = (i < 8) ? pElemCrossNRoadsTop[j == 8] : pElemCrossNRoadsBottom[j == 8];
                    container.emplace_back(row, col);
                } else if (value == 48) pElemNumSize.emplace_back(row, col);
            }
        }
    }

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &inRenderWin) override {
        if (inEvent.type == sf::Event::KeyPressed && inEvent.key.code == sf::Keyboard::Escape)
            rIM_parentMenu->setResponse(-1, 2);
        else if(inEvent.type == sf::Event::KeyPressed && inEvent.key.code == sf::Keyboard::P){
            std::shared_ptr<rIMenu> rProcessing = std::make_shared<rRoadPresentView>(
                    rRoadPresentView(rRLV_rPileMenus->vTopActiveMenu, rRVM_selRoad));
            rRLV_rPileMenus->addMenuTop(rProcessing);
        }

        return false;
    }

    void setResponse(int inValResponse, uint16_t inLIDSender) override {
        rRLV_rPileMenus->removeTop();
    }

    void update() override {
        setSizeRoads(rRVM_selRoad->getSizeRoad());
        setRoadsCars(rRVM_selRoad->getPosRoad(0), rRVM_selRoad->getPosRoad(1));
        setNewOcc(rRVM_selRoad->getOccupancy());
        setNumberComp(rRVM_selRoad->getCapacity());
    }

private:
    std::vector<std::pair<int, int>> pElemOcc;
    std::vector<std::pair<int, int>> pElemNumSize;

    std::vector<std::vector<std::pair<int, int>>> pElemCrossNRoadsTop;
    std::vector<std::vector<std::pair<int, int>>> pElemCrossNRoadsBottom;

    std::vector<uint8_t> gHeightsTop;
    std::vector<uint8_t> gHeightsBottom;

    std::shared_ptr<rPileMenus> rRLV_rPileMenus;
    void setNewOcc(const float fRate) {
        if (fRate < 0 || fRate > 1) return;

        int nTurnOn = (int)(5 * (1 - fRate));
        for (int i = 0; i < pElemOcc.size(); i++) {
            auto quad = &rIM_dInfo[(pElemOcc[i].second + pElemOcc[i].first * rIM_gWidth) * 4];
            for (int k = 0; k < 4; k++)
                quad[k].texCoords = rIM_lRefTiles[(i >= nTurnOn) ? 105 : 32][k];
        }
    }

    void setSizeRoads(const int capRoad) {
        int nToTake = 1 + 2 * (capRoad - 1);
        for (int i = 0; i < pElemCrossNRoadsTop[0].size(); i++) {
            std::vector<sf::Vertex *> sQuadPos = {
                    &rIM_dInfo[(pElemCrossNRoadsTop[0][i].second + pElemCrossNRoadsTop[0][i].first * rIM_gWidth) * 4],
                    &rIM_dInfo[(pElemCrossNRoadsTop[1][i].second + pElemCrossNRoadsTop[1][i].first * rIM_gWidth) * 4],
                    &rIM_dInfo[(pElemCrossNRoadsBottom[0][i].second + pElemCrossNRoadsBottom[0][i].first * rIM_gWidth) * 4],
                    &rIM_dInfo[(pElemCrossNRoadsBottom[1][i].second + pElemCrossNRoadsBottom[1][i].first * rIM_gWidth) * 4]
            };

            for (int k = 0; k < 4; k++)
                for(auto & quad : sQuadPos)
                    quad[k].texCoords = rIM_lRefTiles[(i <= nToTake) ? 32 : 277][k];

            if (i <= nToTake) {
                for (int j = 3; j < 32; j++) {
                    sf::Vertex *quadTop = &rIM_dInfo[(gHeightsBottom[i] * rIM_gWidth + j) * 4];
                    sf::Vertex *quadBottom = &rIM_dInfo[(gHeightsTop[i] * rIM_gWidth + j) * 4];
                    for (int k = 0; k < 4; k++) {
                        quadTop[k].texCoords = rIM_lRefTiles[32][k];
                        quadBottom[k].texCoords = rIM_lRefTiles[32][k];
                    }
                }
            }
        }
    }

    void setNumberComp(const uint16_t rComp) {
        std::string vToShow = uint16_to_padded_string(rComp);
        for (int i = 0; i < 4; i++)
            for (int k = 0; k < 4; k++)
                rIM_dInfo[(pElemNumSize[i].second + pElemNumSize[i].first * rIM_gWidth) * 4 + k].texCoords
                        = rIM_lRefTiles[vToShow[i] - '0' + 48][k];
    }

    void setRoadsCars(const std::vector<std::list<uint32_t>> &rListPosTop,
                      const std::vector<std::list<uint32_t>> &rListPosBottom) {

        std::vector<std::vector<std::list<uint32_t>>> sPos = {rListPosTop, rListPosBottom};
        std::vector<std::vector<uint8_t>> sRefPos = {gHeightsBottom, gHeightsTop};

        for (int j = 0; j < 2; j++)
            for (int i = 0; i < sPos[j].size(); i++)
                for (const auto &p: sPos[j][i])
                    for (int k = 0; k < 4; k++)
                        rIM_dInfo[(sRefPos[j][i] * rIM_gWidth + 3 +
                                   static_cast<int>(
                                           (static_cast<float>(p) / static_cast<float>(rRVM_selRoad->getCapacity())) *
                                           (32 - 3))) * 4 + k].texCoords = rIM_lRefTiles[48][k];
    }
};

#endif //CITYOFWEIRDFISHES_RROADLINEVIEW_H

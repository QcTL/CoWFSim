//
// Created by Laminar on 02/03/2024.
//

#ifndef CITYOFWEIRDFISHES_RGUICLOCK_H
#define CITYOFWEIRDFISHES_RGUICLOCK_H

#include "../../rIMenu.h"
#include "../../../../sim/behaviour/clock/sClockMain.h"

class rGUIClock : public rIMenu {
public:

    explicit rGUIClock(const std::shared_ptr<rIMenu> &mParent,
                       const std::string &pthFileD) : rIMenu(mParent, rRelativePos::pTopLeft, pthFileD) {
        std::vector<std::vector<int>> data = dExtracted;
        for (int i = 0; i < data.size(); ++i) {
            for (int j = 0; j < data[i].size(); ++j) {
                auto row = (rPos == pBottomLeft || rPos == pBottomRight) ? data.size() - 1 - i : i;
                auto col = (rPos == pTopRight || rPos == pBottomRight) ? data[i].size() - 1 - j : j;
                if (data[i][j] == 277 || data[i][j] == 49 || data[i][j] == 50 || data[i][j] == 51) {
                    comPSel.emplace_back(row + 1, col);
                }
            }
        }
        setVelocity(0);
        setClock({0, 0, 0, 0, 0});
    }

    void setClock(const sClockMain::sCM_ClockValues &vNew) {
        std::string gHour = std::to_string((vNew.sCM_rVHour % 12) == 0 ? 12 : (vNew.sCM_rVHour % 12));
        std::string gMinutes = std::to_string(vNew.sCM_rVMinute % 60);
        setText(0, (gHour.size() == 1 ? "0" : "") + gHour +
                   ":" +
                   (gMinutes.size() == 1 ? "0" : "") + gMinutes +
                   ((vNew.sCM_rVHour < 12) ? "am" : "pm"));

        setText(1, std::to_string(vNew.sCM_rVDay + 1 % 32) + '/' + std::to_string(vNew.sCM_rVMonth + 1 % 13));
        setText(2, std::to_string(vNew.sCM_rVYear));
    }

    void setVelocity(const uint8_t &vNew) {
        for (int i = 0; i < comPSel.size(); i++) {
            sf::Vertex *quad = &dInfo[
                    (comPSel[i].second + comPSel[i].first * gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                if (i == vNew)
                    quad[k].texCoords = lRefTiles[30][k];
                else
                    quad[k].texCoords = lRefTiles[32][k];
            }
        }
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(dInfo, &tsTex.tsTex);
    }

    void update() override {}

    void setResponse(int v, uint16_t lID) override {}

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gPressed = getButtonPressed(rWindow, pMouse);
                    if (_gPressed != -1 && _gPressed < 3) {
                        setVelocity(_gPressed);
                        parentMenu->setResponse(_gPressed, 16);
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Num0) {
                    parentMenu->setResponse(0, 16);
                    setVelocity(0);
                } else if (event.key.code == sf::Keyboard::Num1) {
                    parentMenu->setResponse(1, 16);
                    setVelocity(1);
                } else if (event.key.code == sf::Keyboard::Num2) {
                    parentMenu->setResponse(2, 16);
                    setVelocity(2);
                }
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed) override {}

private:
    std::vector<std::pair<uint32_t, uint32_t>> comPSel;

    void setText(const uint8_t tVal, const std::string &cText) {
        for (int i = 0; i < comV[tVal].pLength; i++) {
            sf::Vertex *quad = &dInfo[
                    (comV[tVal].pStartText.second + i + comV[tVal].pStartText.first * gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                if (i >= cText.size())
                    quad[k].texCoords = lRefTiles[32][k];
                else if (cText[i] >= 'a' && cText[i] <= 'z')
                    quad[k].texCoords = lRefTiles[(cText[i] - 'a') + 1][k];
                else if (cText[i] >= 'A' && cText[i] <= 'Z')
                    quad[k].texCoords = lRefTiles[(cText[i] - 'A') + 65][k];
                else if (cText[i] >= '0' && cText[i] <= '9')
                    quad[k].texCoords = lRefTiles[(cText[i] - '0') + 48][k];
                else if (cText[i] == '.')
                    quad[k].texCoords = lRefTiles[46][k];
                else if (cText[i] == ':')
                    quad[k].texCoords = lRefTiles[58][k];
                else if (cText[i] == '/')
                    quad[k].texCoords = lRefTiles[47][k];
                else
                    quad[k].texCoords = lRefTiles[32][k];
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_RGUICLOCK_H

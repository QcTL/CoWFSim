//
// Created by Laminar on 02/03/2024.
//

#ifndef CITYOFWEIRDFISHES_RGUICLOCK_H
#define CITYOFWEIRDFISHES_RGUICLOCK_H

#include "../../rIMenu.h"
#include "../../../../sim/behaviour/clock/sClockMain.h"

class rGUIClock : public rIMenu {
public:

    explicit rGUIClock(const std::shared_ptr<rIMenu> &inMParent, const std::string &inPthFileD)
            : rIMenu(inMParent, rRelativePos::pTopLeft, inPthFileD) {
        std::vector<std::vector<int>> _data = rIM_dExtracted;
        for (int i = 0; i < _data.size(); ++i) {
            for (int j = 0; j < _data[i].size(); ++j) {
                auto row = (rIM_rPos == pBottomLeft || rIM_rPos == pBottomRight) ? _data.size() - 1 - i : i;
                auto col = (rIM_rPos == pTopRight || rIM_rPos == pBottomRight) ? _data[i].size() - 1 - j : j;
                if (_data[i][j] == 277 || _data[i][j] == 49 || _data[i][j] == 50 || _data[i][j] == 51) {
                    rGUIC_comPSel.emplace_back(row + 1, col);
                }
            }
        }
        setVelocity(0);
        setClock({0, 0, 0, 0, 0});
    }

    void setClock(const sClockMain::sCM_ClockValues &inNewValue) {
        rGUIC_lastClock = inNewValue;
        std::string gHour = std::to_string((inNewValue.sCM_rVHour % 12) == 0 ? 12 : (inNewValue.sCM_rVHour % 12));
        std::string gMinutes = std::to_string(inNewValue.sCM_rVMinute % 60);
        setText(0, (gHour.size() == 1 ? "0" : "") + gHour +
                   ":" +
                   (gMinutes.size() == 1 ? "0" : "") + gMinutes +
                   ((inNewValue.sCM_rVHour < 12) ? "am" : "pm"));

        setText(1,
                std::to_string(inNewValue.sCM_rVDay + 1 % 32) + '/' + std::to_string(inNewValue.sCM_rVMonth + 1 % 13));
        setText(2, std::to_string(inNewValue.sCM_rVYear));
    }

    void setVelocity(const uint8_t &inNewValue) {
        for (int i = 0; i < rGUIC_comPSel.size(); i++) {
            sf::Vertex *_quad = &rIM_dInfo[(rGUIC_comPSel[i].second + rGUIC_comPSel[i].first * rIM_gWidth) * 4];
            for (int k = 0; k < 4; k++)
                _quad[k].texCoords = rIM_lRefTiles[i == inNewValue ? 30 : 32][k];
        }
    }

    void update() override {}

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &inRenderWin) override {
        switch (inEvent.type) {
            case sf::Event::MouseButtonPressed:
                if (inEvent.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> _pMouse = sf::Mouse::getPosition(inRenderWin);
                    int _gPressed = getButtonPressed(inRenderWin, _pMouse);
                    if (_gPressed != -1 && _gPressed <= 3) {
                        setVelocity(_gPressed);
                        rIM_parentMenu->setResponse(_gPressed, rIM_idMenu);
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (inEvent.key.code >= sf::Keyboard::Num0 && inEvent.key.code <= sf::Keyboard::Num3) {
                    int velocity = inEvent.key.code - sf::Keyboard::Num0;
                    rIM_parentMenu->setResponse(velocity, rIM_idMenu);
                    setVelocity(velocity);

                    rGUIC_isPaused = inEvent.key.code == sf::Keyboard::Num0;
                    if (inEvent.key.code != sf::Keyboard::Num0)
                        gLastVelocity = velocity;

                } else if (inEvent.key.code == sf::Keyboard::Space) {
                    int velocity = rGUIC_isPaused ? gLastVelocity : 0;
                    rIM_parentMenu->setResponse(velocity, rIM_idMenu);
                    setVelocity(velocity);
                    rGUIC_isPaused = !rGUIC_isPaused;
                }
            default:
                break;
        }
        return false;
    }

    sClockMain::sCM_ClockValues rGUIC_lastClock;
private:
    std::vector<std::pair<uint32_t, uint32_t>> rGUIC_comPSel;
    uint8_t gLastVelocity = 1;
    bool rGUIC_isPaused = true;

    void setText(const uint8_t tVal, const std::string &cText) {
        for (int i = 0; i < rIM_comV[tVal].pLength; i++) {
            sf::Vertex *quad = &rIM_dInfo[
                    (rIM_comV[tVal].pStartText.second + i + rIM_comV[tVal].pStartText.first * rIM_gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                if (i >= cText.size())
                    quad[k].texCoords = rIM_lRefTiles[32][k];
                else if (cText[i] >= 'a' && cText[i] <= 'z')
                    quad[k].texCoords = rIM_lRefTiles[(cText[i] - 'a') + 1][k];
                else if (cText[i] >= 'A' && cText[i] <= 'Z')
                    quad[k].texCoords = rIM_lRefTiles[(cText[i] - 'A') + 65][k];
                else if (cText[i] >= '0' && cText[i] <= '9')
                    quad[k].texCoords = rIM_lRefTiles[(cText[i] - '0') + 48][k];
                else if (cText[i] == '.')
                    quad[k].texCoords = rIM_lRefTiles[46][k];
                else if (cText[i] == ':')
                    quad[k].texCoords = rIM_lRefTiles[58][k];
                else if (cText[i] == '/')
                    quad[k].texCoords = rIM_lRefTiles[47][k];
                else
                    quad[k].texCoords = rIM_lRefTiles[32][k];
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_RGUICLOCK_H

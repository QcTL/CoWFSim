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
                       const std::string &pthFileD) : rIMenu(mParent, rRelativePos::pTopLeft) {
        std::vector<std::vector<int>> data = extractDataFromFile(pthFileD);
        dInfo = getVertexMenu((int) data[0].size(), (int) data.size(), data);
        gWidth = (int) data[0].size();
        gHeight = (int) data.size();

        comV = std::vector<defTxtCompany>(3, {{0, 0}});

        std::vector<uint8_t> sLengths = {9, 5, 4};
        for (int i = 0; i < data.size(); ++i) {
            for (int j = 0; j < data[i].size(); ++j) {
                auto row = (rPos == pBottomLeft || rPos == pBottomRight) ? data.size() - 1 - i : i;
                auto col = (rPos == pTopRight || rPos == pBottomRight) ? data[i].size() - 1 - j : j;
                if (data[i][j] == 65 || data[i][j] == 48) {
                    for (int t = 0; t < comV.size(); ++t) {
                        bool condition = comV[t].pLength == 0;
                        for (int k = 0; k < t; ++k) {
                            condition = condition && (i != comV[k].pStartText.first);
                        }
                        if (condition) {
                            comV[t] = {{row, col}, sLengths[t]};
                            break;
                        }
                    }
                } else if (data[i][j] == 277 || data[i][j] == 49 || data[i][j] == 50 || data[i][j] == 51) {
                    comPSel.emplace_back(row + 1, col);
                }
            }
        }
        setVelocity(0);
        setClock({0, 0, 0, 0, 0});
    }

    //TODO ha de poder cridar al pare per poder canviar la velocitat de reproduccio de la simulacio
    // Tambe el rPileMenu s'haura de alguna manera comunicar amb el simulador per saber quant s'ha de activar la
    //superspeed que canvia molts dels possibles valors que te el sistema.
    //Tambe estaria be que el rGUI no comptes res ell i fos cada cop que hagui de canviar un missatge de rPiles que hauria
    // de venir de algun altre lloc.

    void setClock(const sClockMain::sCM_ClockValues &vNew) {
        std::string gHour = std::to_string((vNew.sCM_rVHour % 12) == 0 ? 12 : (vNew.sCM_rVHour % 12));
        std::string gMinutes = std::to_string(vNew.sCM_rVMinute % 60);
        setText(0, (gHour.size() == 1 ? "0" : "") + gHour +
                   ":" +
                   (gMinutes.size() == 1 ? "0" : "") + gMinutes +
                   ((vNew.sCM_rVHour < 12) ? "am" : "pm"));

        setText(1, std::to_string(vNew.sCM_rVDay % 32) + '/' + std::to_string(vNew.sCM_rVMonth % 13));
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
    struct defTxtCompany {
        std::pair<uint32_t, uint32_t> pStartText;
        uint8_t pLength;
    };
    std::vector<defTxtCompany> comV;

    std::vector<std::pair<uint32_t, uint32_t>> comPSel;

    sf::VertexArray dInfo;
    int gWidth = 0;
    int gHeight = 0;

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

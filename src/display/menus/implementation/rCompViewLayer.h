//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H
#define CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

#include "../rIMenu.h"
#include "../../../sim/structure/obj/sCommon.h"
#include "../oCommonMenus.h"

class rCompViewLayer : public rIMenu {
public:
    explicit rCompViewLayer(const std::shared_ptr<rIMenu> &mParent, const objCompany &rShow,
                            const std::string &pthFileD)
            : rIMenu(mParent, rIMenu::rRelativePos::pTopLeft) {
        std::vector<std::vector<int>> data = extractDataFromFile(pthFileD);

        comV = std::vector<defTxtCompany>(9, {{0, 0}});

        std::vector<uint8_t> sLengths = {11,5,5,13,6, 9,4,9,4,9};

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
                } else if (data[i][j] == 304 || data[i][j] == 305) {
                    pElemSel.emplace_back(row, col);
                    pElemSelAbs.emplace_back(i, j);
                }
            }
        }

        dInfo = getVertexMenu((int) data[0].size(), (int) data.size(), data);
        gWidth = (int) data[0].size();
        gHeight = (int) data.size();


        setText(0,rShow.nName);
        setText(1, oCommonMenus::getCompNumber(rShow.c_cOwn.size()));
        setText(2, oCommonMenus::getCompNumber(rShow.c_cRentedOther.size()));
        setText(3, "1023450");
        setText(4, "25.34");
        if(rShow.c_pOwn.begin() != rShow.c_pOwn.end()) {
            setText(5, std::to_string(rShow.c_pOwn.begin()->first));
            setText(6, oCommonMenus::getCompNumber(rShow.c_pOwn.begin()->second));
        }else{
            setText(5, "");
            setText(6, "");
        }
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(dInfo, &tsTex.tsTex);
    }

    void setResponse(int v) override {}

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    parentMenu->setResponse(-1);
                break;
            case sf::Event::MouseButtonPressed:
                parentMenu->setResponse(-1);
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed) override {}
    uint32_t getType() override{return 2;}
private:
    sf::VertexArray dInfo;
    int gWidth = 0;
    int gHeight = 0;

    std::vector<std::pair<int, int>> pElemSel;
    std::vector<std::pair<int, int>> pElemSelAbs;
    int cCurrenSel = -1;

    struct defTxtCompany {
        std::pair<uint32_t, uint32_t> pStartText;
        uint8_t pLength;
    };
    std::vector<defTxtCompany> comV;

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
                else if(cText[i] == '.')
                    quad[k].texCoords = lRefTiles[46][k];
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

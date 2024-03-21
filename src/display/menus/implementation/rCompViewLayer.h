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

        std::vector<uint8_t> sLengths = {11, 5, 5, 13, 6, 9, 4, 9, 4, 9, 4};

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


        setText(0, rShow.nName);
        setText(1, oCommonMenus::getCompNumber(rShow.c_cActiveLocations.size()));
        setText(2, oCommonMenus::getCompNumber(rShow.c_cRentedLocations.size()));
        setText(3, getFloatToString2Decimal(rShow.c_cActiveFunds));
        setText(4, "25.34");


        setText(5, "");
        setText(6, "");

        setText(7, "");
        setText(8, "");

        if (rShow.c_pOwn.begin() != rShow.c_pOwn.end()) {
            setText(5, std::to_string(rShow.c_pOwn.begin()->first));
            setText(6, oCommonMenus::getCompNumber(rShow.c_pOwn.begin()->second));
        } else {
            setText(5, "");
            setText(6, "");
        }
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(dInfo, &tsTex.tsTex);
    }

    void setResponse(int v, uint16_t lID) override {}

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    parentMenu->setResponse(-1, 1);
                break;
            case sf::Event::MouseButtonPressed:
                //parentMenu->setResponse(-1,1);
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed) override {}

private:

    static std::string getFloatToString2Decimal(const float nUsed) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << nUsed;
        return ss.str();
    }

    std::vector<std::pair<int, int>> pElemSel;
    std::vector<std::pair<int, int>> pElemSelAbs;
    int cCurrenSel = -1;
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

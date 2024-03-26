//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H
#define CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

#include "../../rIMenu.h"
#include "../../../../sim/structure/obj/sCommon.h"
#include "../../oCommonMenus.h"
#include "./rCompViewMoreLayer.h"

class rCompViewLayer : public rIMenu {
public:
    explicit rCompViewLayer(const std::shared_ptr<rIMenu> &mParent, const objCompany &rShow,
                            const std::string &pthFileD, const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(mParent, rIMenu::rRelativePos::pTopLeft), mPiles(mPiles), rCompRef(rShow) {
        std::vector<std::vector<int>> data = extractDataFromFile(pthFileD);

        comV = std::vector<defTxtCompany>(11, {{0, 0}});

        std::vector<uint8_t> sLengths = {11, 5, 5, 13, 6, 9, 4, 9, 4, 9, 4};
        int nSeen = 0;
        for (int i = 0; i < data.size(); ++i) {
            for (int j = 0; j < data[i].size(); ++j) {
                auto row = (rPos == pBottomLeft || rPos == pBottomRight) ? data.size() - 1 - i : i;
                auto col = (rPos == pTopRight || rPos == pBottomRight) ? data[i].size() - 1 - j : j;
                if (data[i][j] == 65 || data[i][j] == 48) {
                    comV[nSeen] = {{row, col}, sLengths[nSeen]};
                    nSeen++;
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
        setText(4, "25.34"); //TODO Change it;

        for (int i = 5; i < 11; i++)
            setText(i, "");
        std::vector<std::pair<uint32_t, int>> pairVec(rShow.c_pOwn.begin(), rShow.c_pOwn.end());

        std::sort(pairVec.begin(), pairVec.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.second > rhs.second;
        });

        for (int i = 0; i < 3 && i < pairVec.size(); ++i)
            if (pairVec.size() > i && (pairVec.begin() + i)->second > 0) {
                auto it = pairVec.begin() + i;
                setText(5 + i * 2, std::to_string(it->first));
                setText(6 + i * 2, oCommonMenus::getCompNumber(it->second));
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
                if (event.key.code == sf::Keyboard::P){
                    std::shared_ptr<rCompViewMoreLayer> rComp = std::make_shared<rCompViewMoreLayer>(
                            rCompViewMoreLayer(mPiles->vTopActiveMenu,
                                           rCompRef,
                                           "d_mCompViewMoreLayer"));
                    mPiles->addMenuTop(rComp);
                }
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
    std::shared_ptr<rPileMenus> mPiles;
    std::vector<std::pair<int, int>> pElemSel;
    std::vector<std::pair<int, int>> pElemSelAbs;
    objCompany rCompRef;
    int cCurrenSel = -1;
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

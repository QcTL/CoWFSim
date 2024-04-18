//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RCELLVIEWMENU_H
#define CITYOFWEIRDFISHES_RCELLVIEWMENU_H

#include "../rIMenu.h"
#include "company/rCompViewLayer.h"

class rCellViewMenu : public rIMenu {
public:
    explicit rCellViewMenu(const std::shared_ptr<rIMenu> &mParent, const std::vector<objCompany> &rShow,
                           const std::shared_ptr<groupEconomy>& sg_groupEconomy,const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(mParent, rIMenu::rRelativePos::pTopLeft, "d_mCellViewLayer"), refPile(mPiles), compShow(rShow) {
        int index = 0;
        for (const auto &it: rShow) {
            if (index < 3)
                setTextCompany(0, it.nName, "100k");
            index++;
        }
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(dInfo, &tsTex.tsTex);
    }

    void setResponse(int v, uint16_t lID) override {}

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    parentMenu->setResponse(-1, 0);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    std::cout << pMouse.x << ":" << pMouse.y << std::endl;
                    std::cout << "TO" << gHeight * 16 << ":" << gWidth * 16 << std::endl;
                    if (isInside(rWindow, gHeight * 16, gWidth * 16, pMouse)) {
                        //Now check if its in the same line as some element select;
                        sf::Vector2<unsigned int> absPos = getAbsPos(rWindow, gHeight * 16, gWidth * 16, pMouse);
                        for (int i = 0; i < pElemSelAbs.size(); i++) {
                            if (pElemSelAbs[i].first * 16 < absPos.y && pElemSelAbs[i].first * 16 + 16 >= absPos.y) {
                                std::shared_ptr<rCompViewLayer> rComp = std::make_shared<rCompViewLayer>(
                                        rCompViewLayer(refPile->vTopActiveMenu, compShow[i], rCVM_groupEconomy,refPile));
                                refPile->addMenuTop(rComp);
                            }
                        }
                    } else {
                        // parentMenu->setResponse(-1);
                    }
                    return true;
                }
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inUTime) override {}

private:

    std::vector<std::pair<int, int>> pElemSel;
    std::vector<std::pair<int, int>> pElemSelAbs;
    std::shared_ptr<groupEconomy> rCVM_groupEconomy;

    struct defTxtCompany {
        struct pWrite {
            std::pair<uint32_t, uint32_t> pStartText;
            uint8_t pLength;
        };
        pWrite nNameCom;
        pWrite nValCom;
    };
    std::vector<defTxtCompany> comV = {{{{0, 0}, 0},
                                               {{0, 0}, 0}},
                                       {{{0, 0}, 0},
                                               {{0, 0}, 0}},
                                       {{{0, 0}, 0},
                                               {{0, 0}, 0}}};

    std::shared_ptr<rPileMenus> refPile;
    std::vector<objCompany> compShow;

    void setTextCompany(const uint8_t tVal, const std::string &cName, const std::string &cValue) {
        for (int i = 0; i < comV[tVal].nNameCom.pLength; i++) {
            sf::Vertex *quad = &dInfo[
                    (comV[tVal].nNameCom.pStartText.second + i + comV[tVal].nNameCom.pStartText.first * gWidth) * 4];
            for (int k = 0; k < 4; k++) {

                if (i >= cName.size())
                    quad[k].texCoords = lRefTiles[32][k];
                else if (cName[i] >= 'a' && cName[i] <= 'z')
                    quad[k].texCoords = lRefTiles[(cName[i] - 'a') + 1][k];
                else if (cName[i] >= 'A' && cName[i] <= 'Z')
                    quad[k].texCoords = lRefTiles[(cName[i] - 'A') + 65][k];
                else if (cName[i] >= '0' && cName[i] <= '9')
                    quad[k].texCoords = lRefTiles[(cName[i] - '0') + 48][k];
            }
        }

        for (int i = 0; i < comV[tVal].nValCom.pLength; i++) {
            sf::Vertex *quad = &dInfo[
                    (comV[tVal].nValCom.pStartText.second + i + comV[tVal].nValCom.pStartText.first * gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                if (i >= cValue.size())
                    quad[k].texCoords = lRefTiles[32][k];
                else if (cValue[i] >= 'a' && cValue[i] <= 'z')
                    quad[k].texCoords = lRefTiles[(cValue[i] - 'a') + 1][k];
                else if (cValue[i] >= 'A' && cValue[i] <= 'Z')
                    quad[k].texCoords = lRefTiles[(cValue[i] - 'A') + 65][k];
                else if (cValue[i] >= '0' && cValue[i] <= '9')
                    quad[k].texCoords = lRefTiles[(cValue[i] - '0') + 48][k];
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_RCELLVIEWMENU_H

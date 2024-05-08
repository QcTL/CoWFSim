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
                           const std::shared_ptr<groupEconomy> &sg_groupEconomy,
                           const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(mParent, rIMenu::rRelativePos::pTopLeft, "d_mCellViewLayer"), refPile(mPiles), compShow(rShow),
              rCVM_groupEconomy(sg_groupEconomy) {
        int index = 0;
        for (const auto &it: rShow) {
            if (index < 3)
                setTextCompany(0, it.nName, "100k");
            index++;
        }
    }

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &inRenderWin) override {
        switch (inEvent.type) {
            case sf::Event::KeyPressed:
                if (inEvent.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
            case sf::Event::MouseButtonPressed:
                if (inEvent.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(inRenderWin);
                    if (isInside(inRenderWin, rIM_gHeight * 16, rIM_gWidth * 16, pMouse)) {
                        sf::Vector2<int> absPos = getRelPosMenu(inRenderWin,pMouse);
                        for (int i = 0; i < pElemSelAbs.size(); i++) {
                            if (pElemSelAbs[i].first * 16 < absPos.y && pElemSelAbs[i].first * 16 + 16 >= absPos.y) {
                                auto rComp = std::make_shared<rCompViewLayer>(
                                        refPile->vTopActiveMenu, compShow[i], rCVM_groupEconomy, refPile);
                                refPile->addMenuTop(rComp);
                            }
                        }
                    }
                    return true;
                }
                break;
            default:
                break;
        }
        return false;
    }

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
    std::vector<defTxtCompany> customCompany = {{{{0, 0}, 0},
                                                        {{0, 0}, 0}},
                                                {{{0, 0}, 0},
                                                        {{0, 0}, 0}},
                                                {{{0, 0}, 0},
                                                        {{0, 0}, 0}}};

    std::shared_ptr<rPileMenus> refPile;
    std::vector<objCompany> compShow;

    void setTextCompany(const uint8_t tVal, const std::string &cName, const std::string &cValue) {
        for (int i = 0; i < customCompany[tVal].nNameCom.pLength; i++) {
            sf::Vertex *quad = &rIM_dInfo[
                    (customCompany[tVal].nNameCom.pStartText.second + i +
                     customCompany[tVal].nNameCom.pStartText.first * rIM_gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                if (i >= cName.size())
                    quad[k].texCoords = rIM_lRefTiles[32][k];
                else if (cName[i] >= 'a' && cName[i] <= 'z')
                    quad[k].texCoords = rIM_lRefTiles[(cName[i] - 'a') + 1][k];
                else if (cName[i] >= 'A' && cName[i] <= 'Z')
                    quad[k].texCoords = rIM_lRefTiles[(cName[i] - 'A') + 65][k];
                else if (cName[i] >= '0' && cName[i] <= '9')
                    quad[k].texCoords = rIM_lRefTiles[(cName[i] - '0') + 48][k];
            }
        }

        for (int i = 0; i < customCompany[tVal].nValCom.pLength; i++) {
            sf::Vertex *quad = &rIM_dInfo[
                    (customCompany[tVal].nValCom.pStartText.second + i +
                     customCompany[tVal].nValCom.pStartText.first * rIM_gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                if (i >= cValue.size())
                    quad[k].texCoords = rIM_lRefTiles[32][k];
                else if (cValue[i] >= 'a' && cValue[i] <= 'z')
                    quad[k].texCoords = rIM_lRefTiles[(cValue[i] - 'a') + 1][k];
                else if (cValue[i] >= 'A' && cValue[i] <= 'Z')
                    quad[k].texCoords = rIM_lRefTiles[(cValue[i] - 'A') + 65][k];
                else if (cValue[i] >= '0' && cValue[i] <= '9')
                    quad[k].texCoords = rIM_lRefTiles[(cValue[i] - '0') + 48][k];
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_RCELLVIEWMENU_H

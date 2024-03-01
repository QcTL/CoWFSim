//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RCELLVIEWMENU_H
#define CITYOFWEIRDFISHES_RCELLVIEWMENU_H

#include "../rIMenu.h"
#include "rCompViewLayer.h"

class rCellViewMenu : public rIMenu {
public:
    explicit rCellViewMenu(const std::shared_ptr<rIMenu> &mParent, const std::vector<obj_company> &rShow,
                           const std::string &pthFileD,
                           const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(mParent, rIMenu::rRelativePos::pTopLeft), refPile(mPiles), compShow(rShow) {
        std::vector<std::vector<int>> data = extractDataFromFile(pthFileD);

        for (int i = 0; i < data.size(); ++i) {
            for (int j = 0; j < data[i].size(); ++j) {
                auto row = (rPos == pBottomLeft || rPos == pBottomRight) ? data.size() - 1 - i : i;
                auto col = (rPos == pTopRight || rPos == pBottomRight) ? data[i].size() - 1 - j : j;
                if (data[i][j] == 65) {
                    for (int t = 0; t < 3; ++t) {
                        bool condition = comV[t].nNameCom.pLength == 0;
                        for (int k = 0; k < t; ++k) {
                            condition = condition && (i != comV[k].nNameCom.pStartText.first);
                        }
                        if (condition) {
                            comV[t].nNameCom = {{row, col}, 11};
                            break;
                        }
                    }
                } else if (data[i][j] == 48) {
                    for (int t = 0; t < 3; ++t) {
                        bool condition = comV[t].nValCom.pLength == 0;
                        for (int k = 0; k < t; ++k) {
                            condition = condition && (i != comV[k].nValCom.pStartText.first);
                        }
                        if (condition) {
                            comV[t].nValCom = {{row, col}, 6};
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

        int index = 0;
        for (const auto &it: rShow) {
            if (index < 3)
                setText(0, it.nName, "100k");
            index++;
        }
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(dInfo, &tsTex.tsTex);
    }

    void setResponse(int v) override {}

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    parentMenu->setResponse(-1);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    std::cout<< pMouse.x << ":" << pMouse.y<< std::endl;
                    std::cout<<"TO" << gHeight * 16 << ":" <<gWidth * 16 << std::endl;
                    if (isInside(rWindow, gHeight * 16, gWidth * 16, pMouse)) {
                        //Now check if its in the same line as some element select;
                        sf::Vector2<unsigned int> absPos = getAbsPos(rWindow, gHeight * 16, gWidth * 16, pMouse);
                        for (int i = 0; i < pElemSelAbs.size(); i++) {
                            if (pElemSelAbs[i].first * 16 < absPos.y && pElemSelAbs[i].first * 16 + 16 >= absPos.y) {
                                std::cout <<"YEP "<< i<<std::endl;
                                std::shared_ptr<rCompViewLayer> rComp = std::make_shared<rCompViewLayer>(
                                        rCompViewLayer(refPile->vTopActiveMenu, compShow[i],
                                        "d_mCompViewLayer"));
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

    void pressedCell(std::pair<int, int> cPressed) override {}
    uint32_t getType() override{return 1;}

private:

    sf::VertexArray dInfo;
    int gWidth = 0;
    int gHeight = 0;

    std::vector<std::pair<int, int>> pElemSel;
    std::vector<std::pair<int, int>> pElemSelAbs;
    int cCurrenSel = -1;

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
    std::vector<obj_company> compShow;

    void setNewSel(int v) {
        sf::Vertex *quad = &dInfo[(pElemSel[cCurrenSel].second + pElemSel[cCurrenSel].first * gWidth) * 4];
        for (int k = 0; k < 4; k++) {
            quad[k].texCoords = lRefTiles[305][k];
        }
        quad = &dInfo[(pElemSel[v].second + pElemSel[v].first * gWidth) * 4];
        for (int k = 0; k < 4; k++) {
            quad[k].texCoords = lRefTiles[304][k];
        }
        cCurrenSel = v;
    }

    void setText(const uint8_t tVal, const std::string &cName, const std::string &cValue) {
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

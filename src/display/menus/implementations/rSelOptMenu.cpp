//
// Created by Laminar on 31/01/2024.
//

#include "rSelOptMenu.h"


rSelOptMenu::rSelOptMenu(const std::shared_ptr<rIMenu> &mParent, int strValue, const std::string &pthFileD,
                         rIMenu::rRelativePos rPos) : rIMenu(mParent, rPos) {
    std::vector<std::vector<int>> data = extractDataFromFile(pthFileD);

    for (int i = 0; i < data.size(); ++i)
        for (int j = 0; j < data[i].size(); ++j)
            if (data[i][j] == 304 || data[i][j] == 305) {
                auto row = (rPos == pBottomLeft || rPos == pBottomRight) ? data.size() - 1 - i : i;
                auto col = (rPos == pTopRight || rPos == pBottomRight) ? data[i].size() - 1 - j : j;
                pElemSel.emplace_back(row, col);
                pElemSelAbs.emplace_back(i, j);
            }
    dInfo = getVertexMenu((int) data[0].size(), (int) data.size(), data);
    gWidth = (int) data[0].size();
    gHeight = (int) data.size();

    setNewSel(strValue);
}

void rSelOptMenu::draw(sf::RenderWindow &rW) {
    rW.draw(dInfo, &tsTex.tsTex);
}

void rSelOptMenu::setNewSel(int v) {
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

void rSelOptMenu::setResponse(int v) {}

bool rSelOptMenu::interact(const sf::Event &event, const sf::RenderWindow &rWindow) {
    switch (event.type) {
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                if (isInside(rWindow, gHeight * 16, gWidth * 16, pMouse)) {
                    //Now check if its in the same line as some element select;
                    sf::Vector2<unsigned int> absPos = getAbsPos(rWindow, gHeight * 16, gWidth * 16, pMouse);
                    for (int i = 0; i < pElemSelAbs.size(); i++) {
                        if (pElemSelAbs[i].first * 16 < absPos.y && pElemSelAbs[i].first * 16 + 16 >= absPos.y) {
                            parentMenu->setResponse(i);
                        }
                    }
                }
                return true;
            }
            break;
        default:
            return false;
    }
}

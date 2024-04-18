//
// Created by Laminar on 31/01/2024.
//

#include "rSelOptMenu.h"


rSelOptMenu::rSelOptMenu(const std::shared_ptr<rIMenu> &mParent, int strValue)
        : rIMenu(mParent, rIMenu::rRelativePos::pBottomRight, "d_mSelectLayer") {
    std::vector<std::vector<int>> data = rIM_dExtracted;

    for (int i = 0; i < data.size(); ++i)
        for (int j = 0; j < data[i].size(); ++j)
            if (data[i][j] == 304 || data[i][j] == 305) {
                auto row = (rIM_rPos == pBottomLeft || rIM_rPos == pBottomRight) ? data.size() - 1 - i : i;
                auto col = (rIM_rPos == pTopRight || rIM_rPos == pBottomRight) ? data[i].size() - 1 - j : j;
                pElemSel.emplace_back(row, col);
                pElemSelAbs.emplace_back(i, j);
            }
    setNewSel(strValue);
}

void rSelOptMenu::draw(sf::RenderWindow &rW) {
    rW.draw(rIM_dInfo, &rIM_tsTex.tsTex);
}

void rSelOptMenu::setNewSel(int v) {
    sf::Vertex *quad = &rIM_dInfo[(pElemSel[cCurrenSel].second + pElemSel[cCurrenSel].first * rIM_gWidth) * 4];
    for (int k = 0; k < 4; k++) {
        quad[k].texCoords = rIM_lRefTiles[305][k];
    }

    quad = &rIM_dInfo[(pElemSel[v].second + pElemSel[v].first * rIM_gWidth) * 4];
    for (int k = 0; k < 4; k++) {
        quad[k].texCoords = rIM_lRefTiles[304][k];
    }

    cCurrenSel = v;
}

void rSelOptMenu::setResponse(int v, uint16_t lID) {}

bool rSelOptMenu::interact(const sf::Event &inEvent, const sf::RenderWindow &inRenderWin) {
    switch (inEvent.type) {
        case sf::Event::MouseButtonPressed:
            if (inEvent.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2<int> pMouse = sf::Mouse::getPosition(inRenderWin);
                if (isInside(inRenderWin, rIM_gHeight * 16, rIM_gWidth * 16, pMouse)) {
                    //Now check if its in the same line as some element select;
                    sf::Vector2<unsigned int> absPos = getAbsPos(inRenderWin, rIM_gHeight * 16, rIM_gWidth * 16,
                                                                 pMouse);
                    for (int i = 0; i < pElemSelAbs.size(); i++) {
                        if (pElemSelAbs[i].first * 16 < absPos.y && pElemSelAbs[i].first * 16 + 16 >= absPos.y) {
                            rIM_parentMenu->setResponse(i, 3);
                        }
                    }
                    return true;
                }
            }
            break;
        default:
            return false;
    }
    return false;
}

//
// Created by Laminar on 31/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RSELOPTMENU_H
#define CITYOFWEIRDFISHES_RSELOPTMENU_H


#include <string>
#include <fstream>
#include "../rIMenu.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>

class rSelOptMenu : public rIMenu {

public:
    explicit rSelOptMenu(const std::shared_ptr<rIMenu> &mParent, int strValue);

    void draw(sf::RenderWindow &rW) override;

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &inRenderWin) override;

    void pressedCell(std::pair<int, int> inCellPressed, uint32_t inPTime, uint32_t inUTime) {}

private:
    void setNewSel(int v);

    std::vector<std::pair<int, int>> pElemSel;
    std::vector<std::pair<int, int>> pElemSelAbs;
    int cCurrenSel = 0;
};


#endif //CITYOFWEIRDFISHES_RSELOPTMENU_H

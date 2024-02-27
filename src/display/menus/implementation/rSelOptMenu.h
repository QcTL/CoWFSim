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
    explicit rSelOptMenu(const std::shared_ptr<rIMenu> &mParent, int strValue, const std::string &pthFileD,
                         rIMenu::rRelativePos rPos);

    void draw(sf::RenderWindow &rW) override;

    void setResponse(int v) override;

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override;

    void pressedCell(std::pair<int, int> cPressed) {}

private:
    void setNewSel(int v);

    std::vector<std::pair<int, int>> pElemSel;
    std::vector<std::pair<int, int>> pElemSelAbs;
    sf::VertexArray dInfo;
    int cCurrenSel = 0;
    int gWidth = 0;
    int gHeight = 0;
};


#endif //CITYOFWEIRDFISHES_RSELOPTMENU_H

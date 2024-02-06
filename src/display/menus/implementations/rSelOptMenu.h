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

class rSelOptMenu : public rIMenu{

public:
    explicit rSelOptMenu(const std::string& pthFileD);
    void draw(sf::RenderWindow &rW) override;

    void setNewSel(int v);

private:
    std::vector<std::pair<int,int>> pElemSel;
    sf::VertexArray dInfo;
    int cCurrenSel = 0;
    int gWidth = 0;
};


#endif //CITYOFWEIRDFISHES_RSELOPTMENU_H

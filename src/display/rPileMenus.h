//
// Created by Laminar on 29/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RPILEMENUS_H
#define CITYOFWEIRDFISHES_RPILEMENUS_H

#include <utility>

#include "menus/rIMenu.h"

class rPileMenus {
public:
    rPileMenus()= default;

    void addMenuTop(const std::shared_ptr<rIMenu>& nMenu) {
        vTopActiveMenu = nMenu;
        vPresentMenus.push_back(nMenu);
    }

    void removeTop(){
        vPresentMenus.pop_back();
        vTopActiveMenu = vPresentMenus.back();
    }

    void display(sf::RenderWindow& rW){
        for(const auto& m: vPresentMenus){
            m->draw(rW);
        }
    }

private:
    std::shared_ptr<rIMenu> vTopActiveMenu;
    std::vector<std::shared_ptr<rIMenu>> vPresentMenus;
};

#endif //CITYOFWEIRDFISHES_RPILEMENUS_H

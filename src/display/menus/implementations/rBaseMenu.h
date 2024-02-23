//
// Created by ganymede on 2/22/24.
//

#ifndef CITYOFWEIRDFISHES_RBASEMENU_H
#define CITYOFWEIRDFISHES_RBASEMENU_H

#include <utility>

#include "../rIMenu.h"
#include "../../rPileMenus.h"
#include "rSelOptMenu.h"

class rBaseMenu : public rIMenu {
public:
    rBaseMenu(std::shared_ptr<rPileMenus> rPile, rRelativePos rRelativePos)
            : rIMenu(nullptr, rRelativePos), refPile(std::move(rPile)), lstValueLayer(1) {
    }

    void draw(sf::RenderWindow &rW) override {}

    void setResponse(int v) override {
        switch (v) {
            case 0:
                refPile->inSim->switchActual(gSimLayersTypes::G_AIRPOLLUTION);
                break;
            case 1:
                refPile->inSim->switchActual(gSimLayersTypes::G_CITY);
                break;
            case 2:
                refPile->inSim->switchActual(gSimLayersTypes::G_TRANSIT);
                break;
            default:
                break;
        }
        lstValueLayer = v;
        refPile->removeTop();
    }

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::M) {
                    std::shared_ptr<rSelOptMenu> rSom = std::make_shared<rSelOptMenu>(
                            refPile->vTopActiveMenu,lstValueLayer ,"d_mSelectLayer", rIMenu::rRelativePos::pBottomRight);
                    refPile->addMenuTop(rSom);
                }
            default:
                break;
        }
        return false;
    }

private:
    std::shared_ptr<rPileMenus> refPile;
    int lstValueLayer;
};

#endif //CITYOFWEIRDFISHES_RBASEMENU_H

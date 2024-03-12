//
// Created by Laminar on 29/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RPILEMENUS_H
#define CITYOFWEIRDFISHES_RPILEMENUS_H

#include <utility>

#include "menus/rIMenu.h"
#include "layers/gDispLayers.h"
#include "menus/implementation/AlwaysOD/rGUIClock.h"

class rPileMenus {
public:
    explicit rPileMenus(const std::shared_ptr<gDispLayers> &gSimL) : inSim(gSimL) {};

    void addMenuTop(const std::shared_ptr<rIMenu> &nMenu) {
        vTopActiveMenu = nMenu;
        vPresentMenus.emplace_back(nMenu->getRPos(), nMenu);
    }

    void removeTop() {
        vPresentMenus.pop_back();
        vTopActiveMenu = vPresentMenus.back().second;
    }

    void display(sf::RenderWindow &rW) {
        rW.setView(rViewTopLeft);
        gClock->draw(rW);

        for (const auto &m: vPresentMenus) {
            switch (m.first) {
                case 0:
                    rW.setView(rViewBottomLeft);
                    break;
                case 1:
                    rW.setView(rViewBottomRight);
                    break;
                case 2:
                    rW.setView(rViewTopLeft);
                    break;
                case 3:
                    rW.setView(rViewTopRight);
                    break;
            }
            m.second->draw(rW);
        }
    }

    void resizedScreen(sf::Event e) {
        int height = e.size.height;
        int width = e.size.width;
        rViewBottomLeft.setSize(e.size.width, -height);
        rViewBottomLeft.setCenter(e.size.width / 2, e.size.height / 2);

        rViewBottomRight.setSize(-width, -height);
        rViewBottomRight.setCenter(e.size.width / 2, e.size.height / 2);

        rViewTopLeft.setSize(e.size.width, height);
        rViewTopLeft.setCenter(e.size.width / 2, e.size.height / 2);

        rViewTopRight.setSize(-width, height);
        rViewTopRight.setCenter(3 * e.size.width / 2, e.size.height / 2);
    }

    void initialize(const sf::RenderWindow &rWindow) {
        int height = rWindow.getSize().y;
        int width = rWindow.getSize().x;
        rViewBottomLeft = sf::View(sf::FloatRect(0, height, width, -height));
        rViewBottomRight = sf::View(sf::FloatRect(width, height, -width, -height));
        rViewTopLeft = sf::View(sf::FloatRect(0, 0, width, height));
        rViewTopRight = sf::View(sf::FloatRect(width, 0, -width, height));
        gClock = std::make_shared<rGUIClock>(vTopActiveMenu, "d_mGUIClock");
    }

    void updateOnEvent(const sf::Event &event, const sf::RenderWindow &rWindow) {
        vTopActiveMenu->interact(event, rWindow);
        gClock->interact(event, rWindow);
    }


    void updateOnLoop(const sf::View &view) {
        vTopActiveMenu->update();
    }

    void pressedCell(std::pair<int, int> cPressed) {
        vTopActiveMenu->pressedCell(cPressed);
    }

    std::shared_ptr<rIMenu> vTopActiveMenu;
    std::shared_ptr<gDispLayers> inSim;
    std::shared_ptr<rGUIClock> gClock;

    double rInteractionGameVel = 0;
private:
    std::vector<uint8_t> vOrderMenus;
    std::vector<std::pair<int, std::shared_ptr<rIMenu>>> vPresentMenus;

    sf::View rViewBottomLeft;
    sf::View rViewBottomRight;
    sf::View rViewTopLeft;
    sf::View rViewTopRight;
};

#endif //CITYOFWEIRDFISHES_RPILEMENUS_H

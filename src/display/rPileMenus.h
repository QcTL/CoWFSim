//
// Created by Laminar on 29/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RPILEMENUS_H
#define CITYOFWEIRDFISHES_RPILEMENUS_H

#include <utility>

#include "menus/rIMenu.h"
#include "layers/gDispLayers.h"
#include "menus/implementation/AlwaysOD/rGUIClock.h"

/**
 * @class rPileMenus
 * @brief This class contains in form of a pile the active menus, with only the top one being interactive
 */
class rPileMenus {
public:
    explicit rPileMenus(const std::shared_ptr<gDispLayers> &gSimL) : inSim(gSimL) {};

    /**
     * @fn  void addMenuTop
     * @brief Add a menu in top of the current pile
     * @param nMenu The new Menu you want to add on top
     */
    void addMenuTop(const std::shared_ptr<rIMenu> &nMenu) {
        vTopActiveMenu = nMenu;
        vPresentMenus.emplace_back(nMenu->getRPos(), nMenu);
    }

    /**
     * @fn void removeTop
     * @brief Remove the menu on top of the pile
     */
    void removeTop() {
        vPresentMenus.pop_back();
        vTopActiveMenu = vPresentMenus.back().second;
    }

    /**
     * @fn display
     * @brief Displays the menus in order insertion in the pile, so the first one will be render on the back, and the newer on the op
     * @param rW The renderWindow where you want to draw the menu
     */
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

    /**
     * @fn resizedScreen
     * @brief resize the current screen where the menus are displayed to be adapted to the new screen
     * @param e The event that calls for a resizing of the application
     */
    void resizedScreen(const sf::Event e) {
        uint32_t height = e.size.height;
        uint32_t width = e.size.width;
        rViewBottomLeft.setSize((float)e.size.width, -(float)height);
        rViewBottomLeft.setCenter((float)e.size.width / 2, (float)e.size.height / 2);

        rViewBottomRight.setSize(-(float)width, -(float)height);
        rViewBottomRight.setCenter((float)e.size.width / 2, (float)e.size.height / 2);

        rViewTopLeft.setSize((float)e.size.width, (float)height);
        rViewTopLeft.setCenter((float)e.size.width / 2, (float)e.size.height / 2);

        rViewTopRight.setSize(-(float)width, (float)height);
        rViewTopRight.setCenter( (float)e.size.width / 2, (float)e.size.height / 2);
    }

    /**
     * @fn void initialize
     * @brief This function is needed to be called on the begging of the simulation to set the views where the menus will be set
     * @param rWindow The render window that the views are set to.
     */
    void initialize(const sf::RenderWindow &rWindow) {
        float height = (float)rWindow.getSize().y;
        float width = (float)rWindow.getSize().x;
        rViewBottomLeft = sf::View(sf::FloatRect(0, height, width, -height));
        rViewBottomRight = sf::View(sf::FloatRect(width, height, -width, -height));
        rViewTopLeft = sf::View(sf::FloatRect(0, 0, width, height));
        rViewTopRight = sf::View(sf::FloatRect(width, 0, -width, height));
        gClock = std::make_shared<rGUIClock>(vTopActiveMenu, "d_mGUIClock");
    }

    /**
     * @fn void updateOnEvent
     * @brief Function called when an event is fired by the graphical part
     * @param event The event that was fired
     * @param rWindow The window which it was displayed
     */
    void updateOnEvent(const sf::Event &event, const sf::RenderWindow &rWindow) {
        vTopActiveMenu->interact(event, rWindow);
        gClock->interact(event, rWindow);
    }

    /**
     * @fn void updateOnLoop
     * @brief Function that should be called once per loop to change the necessary attributes
     * @param view The current view of the renderer
     */
    void updateOnLoop(const sf::View &view) {
        vTopActiveMenu->update();
    }

    /**
     * @fn void pressedCell
     * @brief This function is called when in the graphical part of the simulation a cell is pressed
     * @param cPressed The positive coordinates pressed by the user
     */
    void pressedCell(std::pair<int, int> cPressed) {
        vTopActiveMenu->pressedCell(cPressed,gClock->rGUIC_lastClock.sCM_RTick,gClock->rGUIC_lastClock.sCM_CDate);
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

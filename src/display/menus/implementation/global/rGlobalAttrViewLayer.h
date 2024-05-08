//
// Created by ganymede on 4/6/24.
//

#ifndef CITYOFWEIRDFISHES_RGLOBALATTRVIEWLAYER_H
#define CITYOFWEIRDFISHES_RGLOBALATTRVIEWLAYER_H

#include <memory>
#include "../../rIMenu.h"
#include "../../../../sim/structure/obj/elements/objCompany.h"
#include "../../../rPileMenus.h"
#include "../../../../sim/behaviour/globalAttr/stGlobalTrackerAttr.h"

class rGlobalAttrViewLayer : public rIMenu {
public:
    explicit rGlobalAttrViewLayer(const std::shared_ptr<rIMenu> &mParent)
            : rIMenu(mParent, rIMenu::rRelativePos::pBottomLeft, "d_mGlobalAttrViewLayer") {

        std::shared_ptr<stGlobalTrackerAttr> sGA = stGlobalTrackerAttr::getInstance();
        setText(0, std::to_string(sGA->stGA_totalPopulation));
        setText(1, std::to_string(sGA->stGA_totalEnergy));
        setText(2, std::to_string(sGA->stGA_totalImports));
        setText(3, std::to_string(sGA->stGA_totalExports));
        setText(4, std::to_string(sGA->stGA_ratioEmptyHouses));
    }

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gEyePressed = getEyePressed(rWindow, pMouse);
                    if (_gEyePressed != -1) {
                        std::shared_ptr<stGlobalTrackerAttr> sGA = stGlobalTrackerAttr::getInstance();

                        if (_gEyePressed == 0) {
                            if (!rIM_comVEyesState[0])
                                sGA->stGA_totalPopulation.setObserver(eyeCatcherActive::getInstance());
                            else
                                sGA->stGA_totalPopulation.removeObserver();
                        } else if (_gEyePressed == 1) {
                            if (!rIM_comVEyesState[1])
                                sGA->stGA_totalEnergy.setObserver(eyeCatcherActive::getInstance());
                            else
                                sGA->stGA_totalEnergy.removeObserver();
                        } else if (_gEyePressed == 2) {
                            if (!rIM_comVEyesState[2])
                                sGA->stGA_totalImports.setObserver(eyeCatcherActive::getInstance());
                            else
                                sGA->stGA_totalImports.removeObserver();
                        } else if (_gEyePressed == 3) {
                            if (!rIM_comVEyesState[3])
                                sGA->stGA_totalExports.setObserver(eyeCatcherActive::getInstance());
                            else
                                sGA->stGA_totalExports.removeObserver();
                        } else if (_gEyePressed == 4) {
                            if (!rIM_comVEyesState[4])
                                sGA->stGA_ratioEmptyHouses.setObserver(eyeCatcherActive::getInstance());
                            else
                                sGA->stGA_ratioEmptyHouses.removeObserver();
                        }

                        setEyeVisualValue(_gEyePressed, !rIM_comVEyesState[_gEyePressed]);
                        rIM_comVEyesState[_gEyePressed] = !rIM_comVEyesState[_gEyePressed];
                        return true;
                    }
                }
                break;
            default:
                break;
        }
        return false;
    }

};

#endif //CITYOFWEIRDFISHES_RGLOBALATTRVIEWLAYER_H

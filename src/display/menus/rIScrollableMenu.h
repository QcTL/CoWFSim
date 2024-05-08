//
// Created by ganymede on 4/27/24.
//

#ifndef CITYOFWEIRDFISHES_RISCROLLABLEMENU_H
#define CITYOFWEIRDFISHES_RISCROLLABLEMENU_H

#include "rIMenu.h"

template<typename T>
class rIScrollableMenu : public rIMenu {
public:
    rIScrollableMenu(const std::shared_ptr<rIMenu> &mParent, rRelativePos rRelativePos, const std::string &pthFileD,
                     const int objInPage)
            : rIMenu(mParent, rRelativePos, pthFileD),rPVL_nObjInLayer(objInPage) {
    }


    bool interact(const sf::Event &inEvent, const sf::RenderWindow &rWindow) override {
        switch (inEvent.type) {
            case sf::Event::KeyPressed:
                if (inEvent.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
            case sf::Event::MouseButtonPressed:
                if (inEvent.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gButtonPressed = getButtonPressed(rWindow, pMouse);
                    if (_gButtonPressed != -1) {
                        if (_gButtonPressed == 0)
                            rPVL_cCurLayerObjects = std::max(rPVL_cCurLayerObjects - 1, 0);
                        else if (_gButtonPressed == 1)
                            rPVL_cCurLayerObjects = std::min(rPVL_cCurLayerObjects + 1, (int)rPVL_totalElements.size() /
                                                                                        rPVL_nObjInLayer);
                        printItemsOnScreen();
                    }
                }else if (inEvent.mouseButton.button == sf::Mouse::Right)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
                break;
            default:
                break;
        }
        return false;
    }

protected:
    virtual void printItemsOnScreen() = 0;

    int rPVL_cCurLayerObjects = 0;
    uint16_t rPVL_nObjInLayer = 5;
    std::vector<T> rPVL_totalElements;
};

#endif //CITYOFWEIRDFISHES_RISCROLLABLEMENU_H

//
// Created by ganymede on 3/26/24.
//

#ifndef CITYOFWEIRDFISHES_RCOMPVIEWMORELAYER_H
#define CITYOFWEIRDFISHES_RCOMPVIEWMORELAYER_H

#include <memory>
#include "../../rIMenu.h"
#include "../../../../sim/structure/obj/elements/objCompany.h"
#include "../../../rPileMenus.h"

class rCompViewMoreLayer : public rIMenu {
public:
    explicit rCompViewMoreLayer(const std::shared_ptr<rIMenu> &mParent, objCompany &rShow,
                                const std::string &pthFileD)
            : rIMenu(mParent, rIMenu::rRelativePos::pBottomLeft, pthFileD), rCVML_rShow(rShow) {

        setText(2, getFloatToString2Decimal(rShow.c_objWeek));
        setText(4, getFloatToString2Decimal(rShow.c_objMonth));
        setText(6, getFloatToString2Decimal(rShow.c_objYear));


        setText(3, std::to_string(
                rShow.c_cAvailableByType.find(1) != rShow.c_cAvailableByType.end() ? rShow.c_cAvailableByType.at(
                        (uint8_t) 1) : 0));
        setText(5, std::to_string(
                rShow.c_cAvailableByType.find(2) != rShow.c_cAvailableByType.end() ? rShow.c_cAvailableByType.at(
                        (uint8_t) 2) : 0));
        setText(7, std::to_string(
                rShow.c_cAvailableByType.find(3) != rShow.c_cAvailableByType.end() ? rShow.c_cAvailableByType.at(
                        (uint8_t) 3) : 0));
        setText(9, std::to_string(
                rShow.c_cAvailableByType.find(4) != rShow.c_cAvailableByType.end() ? rShow.c_cAvailableByType.at(
                        (uint8_t) 4) : 0));

        setText(8, std::to_string(rShow.c_nEmployee));
        setText(1, std::to_string(rShow.c_cCode->sCO_Score));
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(rIM_dInfo, &rIM_tsTex.tsTex);
    }

    void setResponse(int v, uint16_t lID) override {}

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, 1);
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gEyePressed = getEyePressed(rWindow, pMouse);
                    if (_gEyePressed != -1) {
                        if (_gEyePressed == 0) {
                            if (!rIM_comVEyesState[0])
                                rCVML_rShow.c_objWeek.setObserver(eyeCatcherActive::getInstance());
                            else
                                rCVML_rShow.c_objWeek.removeObserver();
                        } else if (_gEyePressed == 1) {
                            if (!rIM_comVEyesState[1])
                                rCVML_rShow.c_objMonth.setObserver(eyeCatcherActive::getInstance());
                            else
                                rCVML_rShow.c_objMonth.removeObserver();
                        } else if (_gEyePressed == 2) {
                            if (!rIM_comVEyesState[2])
                                rCVML_rShow.c_objYear.setObserver(eyeCatcherActive::getInstance());
                            else
                                rCVML_rShow.c_objYear.removeObserver();
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

    void pressedCell(std::pair<int, int> cPressed,uint32_t inPTime, uint32_t inUTime) override {}

private:
    objCompany &rCVML_rShow;
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWMORELAYER_H

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


        int sTotalCiv =
                (rShow.c_cAvailableByType.find(sgTerrain::sgT_TypeSoil::sgT_TS_T1Mixed) !=
                 rShow.c_cAvailableByType.end() ? 1 : 0)
                + (rShow.c_cAvailableByType.find(sgTerrain::sgT_TypeSoil::sgT_TS_T2Mixed) !=
                   rShow.c_cAvailableByType.end() ? 1 : 0)
                + (rShow.c_cAvailableByType.find(sgTerrain::sgT_TypeSoil::sgT_TS_T3Mixed) !=
                   rShow.c_cAvailableByType.end() ? 1 : 0);
        setText(3, std::to_string(sTotalCiv));

        setText(5, std::to_string(
                rShow.c_cAvailableByType.find(sgTerrain::sgT_TypeSoil::sgT_TS_T1Industrial) !=
                rShow.c_cAvailableByType.end() ? rShow.c_cAvailableByType.at(
                        (uint8_t) sgTerrain::sgT_TypeSoil::sgT_TS_T1Industrial) : 0));
        setText(7, std::to_string(
                rShow.c_cAvailableByType.find(sgTerrain::sgT_TypeSoil::sgT_TS_T2Industrial) !=
                rShow.c_cAvailableByType.end() ? rShow.c_cAvailableByType.at(
                        (uint8_t) sgTerrain::sgT_TypeSoil::sgT_TS_T2Industrial) : 0));
        setText(9, std::to_string(
                rShow.c_cAvailableByType.find(sgTerrain::sgT_TypeSoil::sgT_TS_T1Farm) != rShow.c_cAvailableByType.end()
                ? rShow.c_cAvailableByType.at((uint8_t) sgTerrain::sgT_TypeSoil::sgT_TS_T1Farm) : 0));

        setText(8, std::to_string(rShow.c_nEmployee));
        setText(1, std::to_string(rShow.c_cCode->sCO_Score));

        rIM_comVEyesState[0] = rCVML_rShow.c_objWeek.isObserved();
        rIM_comVEyesState[1] = rCVML_rShow.c_objMonth.isObserved();
        rIM_comVEyesState[2] = rCVML_rShow.c_objYear.isObserved();
        rIM_comVEyesState[3] = rCVML_rShow.c_nEmployee.isObserved();

        updateEyesCurrentState();
    }

    void updateEyesCurrentState() {
        for (int i = 0; i < 4; i++)
            setEyeVisualValue(i, rIM_comVEyesState[i]);
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
                        } else if (_gEyePressed == 3) {
                            if (!rIM_comVEyesState[3])
                                rCVML_rShow.c_nEmployee.setObserver(eyeCatcherActive::getInstance());
                            else
                                rCVML_rShow.c_nEmployee.removeObserver();
                        }
                        setEyeVisualValue(_gEyePressed, !rIM_comVEyesState[_gEyePressed]);
                        rIM_comVEyesState[_gEyePressed] = !rIM_comVEyesState[_gEyePressed];
                        return true;
                    }
                }else if (inEvent.mouseButton.button == sf::Mouse::Right)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
            default:
                break;
        }
        return false;
    }

private:
    objCompany &rCVML_rShow;
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWMORELAYER_H

//
// Created by ganymede on 4/27/24.
//

#ifndef CITYOFWEIRDFISHES_RROADPRESENTVIEW_H
#define CITYOFWEIRDFISHES_RROADPRESENTVIEW_H

#include "../../rIMenu.h"
#include "../../../../sim/roads/node/rRNode.h"

class rRoadPresentView : public rIMenu {
public:
    rRoadPresentView(const std::shared_ptr<rIMenu> &inMParent, const std::shared_ptr<rRNodeI> &refView)
            : rIMenu(inMParent, rIMenu::rRelativePos::pTopLeft, "d_mRoadsViewPresentLayer"),
            rEPV_refRoad(refView){

        ePVL_totalCars = rEPV_refRoad->exportCarsPresent();
        printItemsOnScreen();
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
                    int _gButtonPressed = getButtonPressed(rWindow, pMouse);
                    if (_gButtonPressed != -1) {
                        if (_gButtonPressed == 0)
                            rPVL_cCurLayerObjects = std::max(rPVL_cCurLayerObjects - 1, 0);
                        else if (_gButtonPressed == 1)
                            rPVL_cCurLayerObjects = std::min(rPVL_cCurLayerObjects + 1, (int)ePVL_totalCars.size() /
                                                                                        rPVL_nObjInLayer);
                        printItemsOnScreen();
                    }
                }
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inUTime) override {}

private:

    void printItemsOnScreen() {
        for (int i = 0; i < rPVL_nObjInLayer * 3; i += 3) {
            uint32_t vObjAct = rPVL_cCurLayerObjects * rPVL_nObjInLayer + i / 3;

            if (vObjAct < ePVL_totalCars.size()) {
                setText(i, std::to_string(ePVL_totalCars[vObjAct].rPV_uuidCar));
                setText(i+1, std::to_string(ePVL_totalCars[vObjAct].rPV_destination.first));
                setText(i+2, std::to_string(ePVL_totalCars[vObjAct].rPV_destination.second));
            } else {
                setText(i, "");
                setText(i + 1, "");
                setText(i + 2, "");
            }
        }
    }

    std::vector<rRNodeL::rPV_Car_Export> ePVL_totalCars;
    int rPVL_cCurLayerObjects = 0;
    uint16_t rPVL_nObjInLayer = 4;
    std::shared_ptr<rRNodeI> rEPV_refRoad;
};

#endif //CITYOFWEIRDFISHES_RROADPRESENTVIEW_H

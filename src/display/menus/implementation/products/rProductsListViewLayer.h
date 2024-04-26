//
// Created by ganymede on 4/21/24.
//

#ifndef CITYOFWEIRDFISHES_RPRODUCTSLISTVIEWLAYER_H
#define CITYOFWEIRDFISHES_RPRODUCTSLISTVIEWLAYER_H

#include "../../rIMenu.h"
#include "../../../../sim/groups/groupEconomy/groupEconomy.h"

class rProductsListViewLayer : public rIMenu {
public:
    explicit rProductsListViewLayer(const std::shared_ptr<rIMenu> &mParent,
                                    const std::shared_ptr<groupEconomy> &inGEcon)
            : rIMenu(mParent, rIMenu::rRelativePos::pBottomLeft, "d_mProductListViewLayer"), rPVL_refGEco(inGEcon) {

        rPVL_totalProductsPrices = rPVL_refGEco->getVecAllItemsPrices();
        for (int i = 0; i < rPVL_totalProductsPrices.size(); i++)
            rPVL_activatedEyesList.push_back(rPVL_refGEco->getStateEyeProduct(i));

        updateEyesCurrentState();
        printItemsOnScreen();
    }

    void updateEyesCurrentState(){
        for (int i = 0; i < rPVL_totalProductsPrices.size(); i++) {
            setEyeVisualValue(0, rPVL_activatedEyesList[i]);
        }
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
                        int _refProduct = _gEyePressed + rPVL_cCurLayerObjects * rPVL_nObjInLayer;
                        rPVL_refGEco->changeStateEyeProduct(_refProduct);

                        rPVL_activatedEyesList[_refProduct] = !rPVL_activatedEyesList[_refProduct];
                        setEyeVisualValue(_gEyePressed, rPVL_activatedEyesList[_refProduct]);
                        return true;
                    }
                    int _gButtonPressed = getButtonPressed(rWindow, pMouse);
                    if (_gButtonPressed != -1) {
                        if (_gButtonPressed == 0)
                            rPVL_cCurLayerObjects = std::max(rPVL_cCurLayerObjects - 1, 0);
                        else if (_gButtonPressed == 1)
                            rPVL_cCurLayerObjects = std::min(rPVL_cCurLayerObjects + 1,
                                                             (int) rPVL_totalProductsPrices.size() /
                                                             rPVL_nObjInLayer);
                        rPVL_totalProductsPrices = rPVL_refGEco->getVecAllItemsPrices();
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
        for (int i = 0; i < rPVL_nObjInLayer * 2; i += 2) {
            uint32_t vObjAct = rPVL_cCurLayerObjects * rPVL_nObjInLayer + i / 2;

            if (vObjAct < rPVL_totalProductsPrices.size()) {
                setText(i, rPVL_totalProductsPrices[vObjAct].first);
                setText(i + 1, std::to_string(rPVL_totalProductsPrices[vObjAct].second));

                setEyeVisualValue(i / 2, rPVL_activatedEyesList[vObjAct]);
            } else {
                setText(i, "");
                setText(i + 1, "");

                setEyeVisualValue(i / 2, false);
            }
        }
    }

    int rPVL_cCurLayerObjects = 0;
    uint16_t rPVL_nObjInLayer = 5;
    std::vector<std::pair<std::string, uint64_t>> rPVL_totalProductsPrices;
    std::vector<bool> rPVL_activatedEyesList;
    std::shared_ptr<groupEconomy> rPVL_refGEco;
};

#endif //CITYOFWEIRDFISHES_RPRODUCTSLISTVIEWLAYER_H

//
// Created by ganymede on 4/21/24.
//

#ifndef CITYOFWEIRDFISHES_RPRODUCTSLISTVIEWLAYER_H
#define CITYOFWEIRDFISHES_RPRODUCTSLISTVIEWLAYER_H

#include "../../rIMenu.h"
#include "../../../../sim/groups/groupEconomy/groupEconomy.h"

class rProductsListViewLayer : public rIScrollableMenu<std::pair<std::string, uint64_t>> {
public:
    explicit rProductsListViewLayer(const std::shared_ptr<rIMenu> &mParent,
                                    const std::shared_ptr<groupEconomy> &inGEcon)
            : rIScrollableMenu<std::pair<std::string, uint64_t>>(mParent, rIMenu::rRelativePos::pBottomLeft,
                                                                 "d_mProductListViewLayer", 5), rPVL_refGEco(inGEcon) {

        rPVL_totalElements = rPVL_refGEco->getVecAllItemsPrices();
        for (int i = 0; i < rPVL_totalElements.size(); i++)
            rPVL_activatedEyesList.push_back(rPVL_refGEco->getStateEyeProduct(i));

        updateEyesCurrentState();
        printItemsOnScreen();
    }


    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        if (rIScrollableMenu<std::pair<std::string, uint64_t>>::interact(event, rWindow))
            return true;

        switch (event.type) {
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
                }
                break;
            default:
                break;
        }
        return false;
    }

protected:
    void updateEyesCurrentState() {
        for (int i = 0; i < rPVL_totalElements.size(); i++) {
            setEyeVisualValue(0, rPVL_activatedEyesList[i]);
        }
    }

    void printItemsOnScreen() override {
        for (int i = 0; i < rPVL_nObjInLayer * 2; i += 2) {
            uint32_t vObjAct = rPVL_cCurLayerObjects * rPVL_nObjInLayer + i / 2;

            if (vObjAct < rPVL_totalElements.size()) {
                setText(i, rPVL_totalElements[vObjAct].first);
                setText(i + 1, std::to_string(rPVL_totalElements[vObjAct].second));
            } else {
                setText(i, "");
                setText(i + 1, "");
            }
            setEyeVisualValue(i / 2, (vObjAct < rPVL_totalElements.size()) && rPVL_activatedEyesList[vObjAct]);
        }

    }

private:
    std::vector<bool> rPVL_activatedEyesList;
    std::shared_ptr<groupEconomy> rPVL_refGEco;
};

#endif //CITYOFWEIRDFISHES_RPRODUCTSLISTVIEWLAYER_H

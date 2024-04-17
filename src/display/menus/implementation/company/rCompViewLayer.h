//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H
#define CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

#include "../../rIMenu.h"
#include "../../../../sim/structure/obj/sCommon.h"
#include "../../oCommonMenus.h"
#include "./rCompViewMoreLayer.h"

class rCompViewLayer : public rIMenu {
public:
    explicit rCompViewLayer(const std::shared_ptr<rIMenu> &mParent, objCompany &rShow,  const std::shared_ptr<groupEconomy> &sgEconomy
                            , const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(mParent, rIMenu::rRelativePos::pTopLeft, "d_mCompViewLayer"), mPiles(mPiles), rCompRef(rShow) {

        setText(0, rShow.nName);
        setText(1, oCommonMenus::getCompNumber(rShow.getNumberActiveCells()));
        setText(2, oCommonMenus::getCompNumber(rShow.getNumberRentedCells()));
        setText(3, getFloatToString2Decimal(rShow.c_cActiveFunds));
        setText(4, getFloatToString2Decimal(
                std::max(-100.0, ((rShow.c_objYear + rShow.c_objMonth * 12 + rShow.c_objWeek * 36) /
                                  rShow.c_cActiveFunds) * 100))); //TODO Change it;

        for (int i = 5; i < 11; i++)
            setText(i, "");
        std::vector<std::pair<uint32_t, int>> pairVec(rShow.c_pOwn.begin(), rShow.c_pOwn.end());

        std::sort(pairVec.begin(), pairVec.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.second > rhs.second;
        });

        for (int i = 0; i < 3 && i < pairVec.size(); ++i)
            if (pairVec.size() > i && (pairVec.begin() + i)->second > 0) {
                auto it = pairVec.begin() + i;
                setText(5 + i * 2, sgEconomy->gE_sEvaluator->getStringByObjId(it->first).substr(3));
                setText(6 + i * 2, oCommonMenus::getCompNumber(it->second));
            }
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(dInfo, &tsTex.tsTex);
    }

    void setResponse(int v, uint16_t lID) override {
        mPiles->removeTop();
    }

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    parentMenu->setResponse(-1, 1);
                if (event.key.code == sf::Keyboard::P) {
                    std::shared_ptr<rCompViewMoreLayer> rComp = std::make_shared<rCompViewMoreLayer>(
                            rCompViewMoreLayer(mPiles->vTopActiveMenu,
                                               rCompRef,
                                               "d_mCompViewMoreLayer"));
                    mPiles->addMenuTop(rComp);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gEyePressed = getEyePressed(rWindow, pMouse);
                    if (_gEyePressed != -1) {
                        if (_gEyePressed == 0) {
                            if (!comVEyesState[0])
                                rCompRef.c_cActiveFunds.setObserver(eyeCatcherActive::getInstance());
                            else
                                rCompRef.c_cActiveFunds.removeObserver();
                            setEyeVisualValue(0, !comVEyesState[0]);
                            comVEyesState[0] = !comVEyesState[0];
                        }
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

    std::shared_ptr<rPileMenus> mPiles;
    objCompany &rCompRef;
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

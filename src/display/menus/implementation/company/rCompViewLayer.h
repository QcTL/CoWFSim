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
    explicit rCompViewLayer(const std::shared_ptr<rIMenu> &inMParent, objCompany &inSelCompany,
                            const std::shared_ptr<groupEconomy> &inSGEconomy, const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(inMParent, rIMenu::rRelativePos::pTopLeft, "d_mCompViewLayer"), mPiles(mPiles),
              rCompRef(inSelCompany) {

        setText(0, inSelCompany.nName);
        setText(1, oCommonMenus::getCompNumber((int) inSelCompany.getNumberActiveCells()));
        setText(2, oCommonMenus::getCompNumber((int) inSelCompany.getNumberRentedCells()));
        setText(3, getFloatToString2Decimal((float) inSelCompany.c_cActiveFunds));
        setText(4, getFloatToString2Decimal(
                (float) std::max(-100.0, ((inSelCompany.c_objYear + inSelCompany.c_objMonth * 12 +
                                           inSelCompany.c_objWeek * 36) /
                                          inSelCompany.c_cActiveFunds) * 100)));

        for (int i = 5; i < 11; i++)
            setText(i, "");
        std::vector<std::pair<uint32_t, int>> pairVec(inSelCompany.c_pOwn.begin(), inSelCompany.c_pOwn.end());

        std::sort(pairVec.begin(), pairVec.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.second > rhs.second;
        });

        for (int i = 0; i < 3 && i < pairVec.size(); ++i)
            if (pairVec.size() > i && (pairVec.begin() + i)->second > 0) {
                auto it = pairVec.begin() + i;
                setText(5 + i * 2, inSGEconomy->gE_sEvaluator->getStringByObjId(it->first));
                setText(6 + i * 2, oCommonMenus::getCompNumber(it->second));
            }
    }

    void draw(sf::RenderWindow &inRenderWin) override {
        inRenderWin.draw(rIM_dInfo, &rIM_tsTex.tsTex);
    }

    void setResponse(int inValResponse, uint16_t inLIDSender) override {
        mPiles->removeTop();
    }

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &rWindow) override {
        switch (inEvent.type) {
            case sf::Event::KeyPressed:
                if (inEvent.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, 1);
                if (inEvent.key.code == sf::Keyboard::P) {
                    std::shared_ptr<rCompViewMoreLayer> rComp = std::make_shared<rCompViewMoreLayer>(
                            rCompViewMoreLayer(mPiles->vTopActiveMenu,
                                               rCompRef,
                                               "d_mCompViewMoreLayer"));
                    mPiles->addMenuTop(rComp);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (inEvent.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gEyePressed = getEyePressed(rWindow, pMouse);
                    if (_gEyePressed != -1) {
                        if (_gEyePressed == 0) {
                            if (!rIM_comVEyesState[0])
                                rCompRef.c_cActiveFunds.setObserver(eyeCatcherActive::getInstance());
                            else
                                rCompRef.c_cActiveFunds.removeObserver();
                            setEyeVisualValue(0, !rIM_comVEyesState[0]);
                            rIM_comVEyesState[0] = !rIM_comVEyesState[0];
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

    void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inUTime) override {}

private:

    std::shared_ptr<rPileMenus> mPiles;
    objCompany &rCompRef;
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

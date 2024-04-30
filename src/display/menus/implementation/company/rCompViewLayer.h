//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H
#define CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

#include "../../rIMenu.h"
#include "../../../../sim/structure/obj/sCommon.h"
#include "../../oCommonMenus.h"
#include "./rCompViewMoreLayer.h"
#include "rCompPaymentsViewLayer.h"
#include "rCompViewProcessing.h"

class rCompViewLayer : public rIMenu {
public:
    explicit rCompViewLayer(const std::shared_ptr<rIMenu> &inMParent, objCompany &inSelCompany,
                            const std::shared_ptr<groupEconomy> &inSGEconomy, const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(inMParent, rIMenu::rRelativePos::pTopLeft, "d_mCompViewLayer"), mPiles(mPiles),
              rCompRef(inSelCompany), rCVL_SGEconomy(inSGEconomy) {

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

        updateEyesCurrentState();
    }

    void updateEyesCurrentState() {
        setEyeVisualValue(0, rCompRef.c_cActiveFunds.isObserved());
    }

    void draw(sf::RenderWindow &inRenderWin) override {
        inRenderWin.draw(rIM_dInfo, &rIM_tsTex.tsTex);
    }

    void setResponse(int inValResponse,const std::string& inLIDSender) override {
        mPiles->removeTop();
    }

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &rWindow) override {
        switch (inEvent.type) {
            case sf::Event::KeyPressed:
                if (inEvent.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                else if (inEvent.key.code == sf::Keyboard::P) {
                    std::shared_ptr<rIMenu> rComp = std::make_shared<rCompViewMoreLayer>(
                            rCompViewMoreLayer(mPiles->vTopActiveMenu,
                                               rCompRef,
                                               "d_mCompViewMoreLayer"));
                    mPiles->addMenuTop(rComp);
                } else if (inEvent.key.code == sf::Keyboard::H) {
                    std::shared_ptr<rIMenu> rComp = std::make_shared<rCompPaymentsViewLayer>(
                            rCompPaymentsViewLayer(mPiles->vTopActiveMenu,
                                                   rCompRef));
                    mPiles->addMenuTop(rComp);
                } else if (inEvent.key.code == sf::Keyboard::C) {
                    std::shared_ptr<rIMenu> rProcessing = std::make_shared<rCompViewProcessing>(
                            rCompViewProcessing(mPiles->vTopActiveMenu, rCompRef, rCVL_SGEconomy));
                    mPiles->addMenuTop(rProcessing);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (inEvent.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gEyePressed = getEyePressed(rWindow, pMouse);
                    if (_gEyePressed != -1) {
                        if (_gEyePressed == 0) {
                            if (!rCompRef.c_cActiveFunds.isObserved())
                                rCompRef.c_cActiveFunds.setObserver(eyeCatcherActive::getInstance());
                            else
                                rCompRef.c_cActiveFunds.removeObserver();
                            setEyeVisualValue(0, rCompRef.c_cActiveFunds.isObserved());
                        }
                        return true;
                    }
                } else if (inEvent.mouseButton.button == sf::Mouse::Right)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inUTime) override {}

private:

    std::shared_ptr<rPileMenus> mPiles;
    std::shared_ptr<groupEconomy> rCVL_SGEconomy;
    objCompany &rCompRef;
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

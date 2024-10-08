//
// Created by ganymede on 3/30/24.
//

#ifndef CITYOFWEIRDFISHES_RHOMEVIEWLAYER_H
#define CITYOFWEIRDFISHES_RHOMEVIEWLAYER_H

#include "../../rIMenu.h"
#include "../../../../sim/structure/obj/sCommon.h"
#include "../../oCommonMenus.h"
#include "../../../rPileMenus.h"
#include "../company/rCompViewLayer.h"

class rHomeViewLayer : public rIMenu {
public:
    explicit rHomeViewLayer(const std::shared_ptr<rIMenu> &mParent, const std::shared_ptr<objCompany> &rShow,
                            const std::shared_ptr<groupLand>& sg_groupLand, std::pair<int,int> cPressed,
                            const std::shared_ptr<groupEconomy>& sg_groupEconomy,
                            const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(mParent, rIMenu::rRelativePos::pBottomRight, "d_mHomeViewLayer"), mPiles(mPiles), rCompRef(rShow),
              rHVL_groupEconomy(sg_groupEconomy){
        switch (sg_groupLand->gL_gTerrain->gTG_TypeSoil->get(cPressed)) {
            case 3:
                setText(0, "Small H");
                break;
            case 2:
                setText(0, "Medium H");
                break;
            case 1:
                setText(0, "Large H");
                break;
        }
        setText(1, std::to_string(sg_groupLand->gL_gTerrain->gTG_civilOccupancy->get(cPressed)));
        setText(2, std::to_string(sg_groupLand->gL_gTerrain->getMaxOccByPos(cPressed)));
        setText(3, std::to_string( sg_groupLand->gL_gTerrain->gTG_baseQualityAttr->get(cPressed)));

        setText(4, rShow != nullptr ? rShow->nName : "");
    }

    void setResponse(int v,const std::string& lID) override {
        mPiles->removeTop();
    }

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &rWindow) override {
        switch (inEvent.type) {
            case sf::Event::MouseButtonPressed:
                if (inEvent.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gPressed = getButtonPressed(rWindow, pMouse);
                    if (_gPressed != -1) {
                        if (_gPressed == 0 && rCompRef != nullptr) {
                            std::shared_ptr<rCompViewLayer> rComp = std::make_shared<rCompViewLayer>(
                                    rCompViewLayer(mPiles->vTopActiveMenu,
                                                   *rCompRef, rHVL_groupEconomy, mPiles));
                            mPiles->addMenuTop(rComp);
                        }
                    }
                }else if (inEvent.mouseButton.button == sf::Mouse::Right)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
            case sf::Event::KeyPressed:
                if (inEvent.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                if (inEvent.key.code == sf::Keyboard::C && rCompRef != nullptr) {
                    std::shared_ptr<rCompViewLayer> rComp = std::make_shared<rCompViewLayer>(
                            rCompViewLayer(mPiles->vTopActiveMenu,
                                           *rCompRef,rHVL_groupEconomy, mPiles));
                    mPiles->addMenuTop(rComp);
                }
                break;
            default:
                break;
        }
        return false;
    }

private:

    std::shared_ptr<rPileMenus> mPiles;
    std::vector<std::pair<int, int>> pElemSel;
    std::vector<std::pair<int, int>> pElemSelAbs;
    std::shared_ptr<objCompany> rCompRef;

    std::shared_ptr<groupEconomy> rHVL_groupEconomy;
};

#endif //CITYOFWEIRDFISHES_RHOMEVIEWLAYER_H

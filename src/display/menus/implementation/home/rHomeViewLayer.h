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
                            uint8_t nOccupancy, uint8_t typeHouse, const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(mParent, rIMenu::rRelativePos::pBottomRight, "d_mHomeViewLayer"), mPiles(mPiles), rCompRef(rShow) {
        switch (typeHouse) {
            case 3:
                setText(0, "Small H");
                setText(2, "15");
                break;
            case 2:
                setText(0, "Medium H");
                setText(2, "20"); // TODO Make it standard.
                break;
            case 1:
                setText(0, "Large H");
                setText(2, "50");
                break;
        }
        setText(1, std::to_string(nOccupancy));

        setText(4, rShow != nullptr ? rShow->nName : "");
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
                if (event.key.code == sf::Keyboard::C && rCompRef != nullptr) {
                    std::shared_ptr<rCompViewLayer> rComp = std::make_shared<rCompViewLayer>(
                            rCompViewLayer(mPiles->vTopActiveMenu,
                                           *rCompRef, "d_mCompViewLayer", mPiles));
                    mPiles->addMenuTop(rComp);
                }
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed) override {}

private:

    std::shared_ptr<rPileMenus> mPiles;
    std::vector<std::pair<int, int>> pElemSel;
    std::vector<std::pair<int, int>> pElemSelAbs;
    std::shared_ptr<objCompany> rCompRef;
};

#endif //CITYOFWEIRDFISHES_RHOMEVIEWLAYER_H

//
// Created by ganymede on 2/24/24.
//

#ifndef CITYOFWEIRDFISHES_RROADVIEWMENU_H
#define CITYOFWEIRDFISHES_RROADVIEWMENU_H

#include "../../rIMenu.h"
#include "../../../../sim/roads/node/rRNode.h"

std::string uint16_to_padded_string(const uint16_t num) {
    std::string str = std::to_string(num);
    if (str.length() > 4) {
        str = str.substr(0, 4);
    } else {
        while (str.length() < 4) {
            str.insert(0, "0");
        }
    }
    return str;
}

class rRoadViewMenu : public rIMenu {
public:
    explicit rRoadViewMenu(const std::shared_ptr<rIMenu> &mParent, const std::shared_ptr<rRNodeI> &refView,
                           const std::string &pthFileD)
            : rIMenu(mParent, rIMenu::rRelativePos::pBottomLeft, pthFileD), rRVM_selRoad(refView) {
    }

    void update() override {}

    void draw(sf::RenderWindow &inRenderWin) override {
        inRenderWin.draw(rIM_dInfo, &rIM_tsTex.tsTex);
    }

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &inRenderWin) override {
        if (inEvent.type == sf::Event::KeyPressed && inEvent.key.code == sf::Keyboard::Escape)
            rIM_parentMenu->setResponse(-1, rIM_idMenu);
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inUTime) override {
        rIM_parentMenu->setResponse(-1, rIM_idMenu);
    }

protected:
    std::shared_ptr<rRNodeI> rRVM_selRoad;
};

#endif //CITYOFWEIRDFISHES_RROADVIEWMENU_H

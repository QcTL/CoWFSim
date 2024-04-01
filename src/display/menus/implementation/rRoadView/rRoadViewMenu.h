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
                           const std::string &pthFileD, rIMenu::rRelativePos rPos)
            : rIMenu(mParent, rPos, pthFileD), rSelRoad(refView) {
    }

    void update() override {}

    void draw(sf::RenderWindow &rW) override {
        rW.draw(dInfo, &tsTex.tsTex);
    }

    void setResponse(int v, uint16_t lID) override {}

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    parentMenu->setResponse(-1,2);
                }
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed) override{
        parentMenu->setResponse(-1,2);
    }

protected:
    std::shared_ptr<rRNodeI> rSelRoad;
};

#endif //CITYOFWEIRDFISHES_RROADVIEWMENU_H

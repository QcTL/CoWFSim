//
// Created by ganymede on 4/27/24.
//

#ifndef CITYOFWEIRDFISHES_RCOMPVIEWPROCESSING_H
#define CITYOFWEIRDFISHES_RCOMPVIEWPROCESSING_H

#include "../../rIMenu.h"
#include "../../../../sim/structure/obj/elements/objCompany.h"
#include "../../../../sim/behaviour/clock/sClockMain.h"
#include "../../../../sim/groups/groupEconomy/groupEconomy.h"

class rCompViewProcessing : public rIMenu {
public:
    explicit rCompViewProcessing(const std::shared_ptr<rIMenu> &mParent, objCompany &rShow,
                                 const std::shared_ptr<groupEconomy> &inGEcon)
            : rIMenu(mParent, rIMenu::rRelativePos::pBottomLeft, "d_mCompViewProcessingLayer") {

        setText(2, getFloatToString2Decimal(rShow.c_objWeek));

        std::vector<std::pair<uint32_t, uint32_t>> _rTopProcessing;
        int _count = 0;
        for (const auto &pair: rShow.c_cActiveProcessing) {
            for (const auto &elem: pair.second) {
                if (_count <= 4)
                    _rTopProcessing.emplace_back(pair.first, elem);
                else
                    break;
                _count++;
            }
            if (_count > 4)
                break;
        }

        for (int i = 0; i < 4 * 2; i += 2) {
            if (_rTopProcessing.size() > i / 2) {
                sClockMain::sCM_ClockValues sDateVal = sClockMain::unpackDateInfo(_rTopProcessing[i / 2].first);
                std::string tDate =
                        ((sDateVal.sCM_rVDay + 1) < 10 ? "0" : "") + std::to_string(sDateVal.sCM_rVDay + 1) + '/' +
                        ((sDateVal.sCM_rVMonth + 1) < 10 ? "0" : "") + std::to_string(sDateVal.sCM_rVMonth + 1);
                setText(i + 1, tDate);
                setText(i, inGEcon->getNameById(_rTopProcessing[i / 2].second));
            } else {
                setText(i + 1, "");
                setText(i, "");
            }
        }
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(rIM_dInfo, &rIM_tsTex.tsTex);
    }

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &rWindow) override {
        switch (inEvent.type) {
            case sf::Event::KeyPressed:
                if (inEvent.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
            case sf::Event::MouseButtonPressed:
                if (inEvent.mouseButton.button == sf::Mouse::Right)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inUTime) override {}
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWPROCESSING_H

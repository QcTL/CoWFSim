//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RSTATIONVIEWLAYER_H
#define CITYOFWEIRDFISHES_RSTATIONVIEWLAYER_H

#include "../../rIMenu.h"
#include "../../../../sim/structure/obj/sCommon.h"
#include "../../oCommonMenus.h"
#include "../../../../sim/structure/obj/elements/objCompany.h"
#include "../../../rPileMenus.h"
#include "../../../../sim/groups/groupLand/sgUndergroundMain.h"
#include "../../../../sim/roads/sgRoadsMain.h"

class rStationViewLayer : public rIMenu {
public:
    explicit rStationViewLayer(const std::shared_ptr<rIMenu> &mParent,
                               std::shared_ptr<sgUndergroundMain> &sgUnderground, int inUuidStation, uint32_t inRTime,
                               uint32_t inCDate, const std::shared_ptr<sgRoadsMain> &inSgRoads,
                               const std::string &pthFileD, const std::shared_ptr<rPileMenus> &mPiles)
            : rIMenu(mParent, rIMenu::rRelativePos::pTopLeft, pthFileD), mPiles(mPiles), soUnderground(sgUnderground) {

        uint32_t nextTime = sgUnderground->getClosestTimeForStation(inUuidStation, inRTime);
        uint32_t prevTime = sgUnderground->getPastClosestTimeForStation(inUuidStation, inRTime);

        std::string gPrevHour = std::to_string(((prevTime/12)) == 0 ? 12 : ((prevTime/12)));
        std::string gPrevMinutes = std::to_string((prevTime%12) * 5);
        setText(0, (gPrevHour.size() == 1 ? "0" : "") + gPrevHour +
                   ":" +
                   (gPrevMinutes.size() == 1 ? "0" : "") + gPrevMinutes +
                   (((prevTime/12) < 12) ? "am" : "pm"));

        std::string gNextHour = std::to_string(((nextTime/12)) == 0 ? 12 : ((nextTime/12)));
        std::string gNextMinutes = std::to_string((nextTime%12) * 5);
        setText(1, (gNextHour.size() == 1 ? "0" : "") + gNextHour +
                   ":" +
                   (gNextMinutes.size() == 1 ? "0" : "") + gNextMinutes +
                   (((nextTime/12) < 12) ? "am" : "pm"));

        setText(2, std::to_string(
                (int) inSgRoads->getRoutesByType(objCivil::typeRouteSystem::OC_TRS_TRAIN, nextTime,
                                                 inCDate).size()));
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(rIM_dInfo, &rIM_tsTex.tsTex);
    }

    void setResponse(int v,const std::string& lID) override {
        mPiles->removeTop();
    }

    bool interact(const sf::Event &inEvent, const sf::RenderWindow &rWindow) override {
        switch (inEvent.type) {
            case sf::Event::KeyPressed:
                if (inEvent.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
            case sf::Event::MouseButtonPressed:
                if (inEvent.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gEyePressed = getEyePressed(rWindow, pMouse);
                    if (_gEyePressed != -1) {
                        if (_gEyePressed == 0) {
                            /*if (!rIM_comVEyesState[0])
                                soUnderground.c_cActiveFunds.setObserver(eyeCatcherActive::getInstance());
                            else
                                soUnderground.c_cActiveFunds.removeObserver();
                            */
                             setEyeVisualValue(0, !rIM_comVEyesState[0]);
                            rIM_comVEyesState[0] = !rIM_comVEyesState[0];
                        }
                        return true;
                    }
                } else if (inEvent.mouseButton.button == sf::Mouse::Right)
                    rIM_parentMenu->setResponse(-1, rIM_idMenu);
                break;
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inUTime) override {}

private:

    std::shared_ptr<rPileMenus> mPiles;
    std::shared_ptr<sgUndergroundMain> &soUnderground;
};

#endif //CITYOFWEIRDFISHES_RCOMPVIEWLAYER_H

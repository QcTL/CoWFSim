//
// Created by ganymede on 2/22/24.
//

#ifndef CITYOFWEIRDFISHES_RBASEMENU_H
#define CITYOFWEIRDFISHES_RBASEMENU_H

#include <utility>

#include "../rIMenu.h"
#include "../../rPileMenus.h"
#include "rSelOptMenu.h"
#include "rRoadViewMenu.h"
#include "rCellViewMenu.h"

class rBaseMenu : public rIMenu {
public:
    explicit rBaseMenu(const std::shared_ptr<rPileMenus> &rPile,
                       const std::shared_ptr<gIGrid<uint8_t>> &gType,
                       const std::vector<std::vector<rNode *>> &gRoads,
                       const std::shared_ptr<gIGrid<std::list<uint32_t>>> &gLayerOwn,
                       const std::shared_ptr<sTotalCompany> &sTComp)
            : rIMenu(nullptr, rIMenu::rRelativePos::pBottomRight),
              refPile(rPile), lstValueLayer(1),
              refLRoads(gRoads), refLTypes(gType), refLBuild(gLayerOwn), refSComp(sTComp) {
    }

    void draw(sf::RenderWindow &rW) override {}

    void setResponse(int v, uint16_t lID) override {
        if (lID == 3) {
            switch (v) {
                case 0:
                    refPile->inSim->switchActual(gSimLayersTypes::G_AIRPOLLUTION);
                    break;
                case 1:
                    refPile->inSim->switchActual(gSimLayersTypes::G_CITY);
                    break;
                case 3:
                    refPile->inSim->switchActual(gSimLayersTypes::G_TRANSIT);
                    break;
                default:
                    break;
            }
            lstValueLayer = v;
        } else if (lID == 16) {
            switch (v) {
                case 0:
                    refPile->rInteractionGameVel = 0.0;
                    break;
                case 1:
                    refPile->rInteractionGameVel = 1000.0;
                    break;
                case 2:
                    refPile->rInteractionGameVel = 100.0;
                    break;
                default:
                    break;
            }
        }
        if (lID < 16)
            refPile->removeTop();
    }

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::M) {
                    std::shared_ptr<rSelOptMenu> rSom = std::make_shared<rSelOptMenu>(
                            refPile->vTopActiveMenu, lstValueLayer, "d_mSelectLayer",
                            rIMenu::rRelativePos::pBottomRight);
                    refPile->addMenuTop(rSom);
                } else if (event.key.code == sf::Keyboard::S) {
                    std::shared_ptr<rRoadViewMenu> rRoad = std::make_shared<rRoadViewMenu>(
                            refPile->vTopActiveMenu, nullptr, "d_mRoadsViewLayer", rIMenu::rRelativePos::pTopLeft);
                    refPile->addMenuTop(rRoad);
                }
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed) override {
        std::cout << cPressed.first << ":" << cPressed.second << std::endl;
        switch (refLTypes->get(cPressed)) {
            case 1: {
                if (!refLBuild->get(cPressed).empty()) {
                    std::shared_ptr<rCellViewMenu> rComp = std::make_shared<rCellViewMenu>(
                            rCellViewMenu(refPile->vTopActiveMenu,
                                          refSComp->getVectCompByUUID(refLBuild->get(cPressed)),
                                          "d_mCellViewLayer", refPile));
                    refPile->addMenuTop(rComp);
                }
            }
                break;
            case 2:
                if (refLRoads[cPressed.first][cPressed.second] != nullptr) {
                    std::shared_ptr<rRoadViewMenu> rRoad = std::make_shared<rRoadViewMenu>(
                            refPile->vTopActiveMenu, refLRoads[cPressed.first][cPressed.second]->refCompressed,
                            "d_mRoadsViewLayer", rIMenu::rRelativePos::pTopLeft);
                    refPile->addMenuTop(rRoad);
                }
                break;
        }
    }


private:
    std::shared_ptr<rPileMenus> refPile;
    int lstValueLayer;

    std::vector<std::vector<rNode *>> refLRoads;
    std::shared_ptr<gIGrid<uint8_t>> refLTypes;
    std::shared_ptr<gIGrid<std::list<uint32_t>>> refLBuild;
    std::shared_ptr<sTotalCompany> refSComp;
};

#endif //CITYOFWEIRDFISHES_RBASEMENU_H
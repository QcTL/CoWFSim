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

struct cellInf {
    enum CellInf_TypeCell {
        CELLINF_TYPE1_MIXED,
        CELLINF_TYPE1_EXCLUSIVE,
        CELLINF_TYPE2_MIXED,
        CELLINF_TYPE2_EXCLUSIVE,
        CELLINF_TYPE3_MIXED,
        CELLINF_TYPE3_EXCLUSIVE,
        CELLINF_TYPE4_INDUSTRIAL,
        CELLINF_TYPE5_FARMLAND,
        CELLINF_TYPE6_PROTECTED,
    };
    struct CellInf_Owner {
        uint8_t o_type;
        uint32_t o_uuid;
    };

    CellInf_TypeCell ci_type;
    CellInf_Owner ci_owner;
};

class rBaseMenu : public rIMenu {
public:
    rBaseMenu(const std::shared_ptr<rPileMenus> &rPile,
              const std::shared_ptr<gIGrid<uint8_t>> &gType,
              const std::vector<std::vector<rNode *>> &gRoads,
              const std::shared_ptr<gIGrid<std::list<uint32_t>>> &gLayerOwn,
              const std::shared_ptr<sTotalCompany> &sTComp)
            : rIMenu(nullptr, rIMenu::rRelativePos::pBottomRight),
              refPile(rPile), lstValueLayer(1),
              refLRoads(gRoads), refLTypes(gType), refLBuild(gLayerOwn), refSComp(sTComp) {
    }

    void draw(sf::RenderWindow &rW) override {}

    void setResponse(int v) override {

        //TODO CHENGE IT SO ITS ONLY WHEN ITS THE TIME
        switch (v) {
            case -1:
                break;
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

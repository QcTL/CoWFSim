//
// Created by ganymede on 2/22/24.
//

#ifndef CITYOFWEIRDFISHES_RBASEMENU_H
#define CITYOFWEIRDFISHES_RBASEMENU_H

#include <utility>

#include "../rIMenu.h"
#include "../../rPileMenus.h"
#include "rSelOptMenu.h"
#include "rRoadView/rRoadViewMenu.h"
#include "rCellViewMenu.h"
#include "rRoadView/rRoadLineView.h"
#include "rRoadView/rRoadCrossView.h"
#include "home/rHomeViewLayer.h"
#include "global/rGlobalAttrViewLayer.h"

class rBaseMenu : public rIMenu {
public:
    explicit rBaseMenu(const std::shared_ptr<rPileMenus> &rPile,
                       const std::shared_ptr<sgTerrain> &sgTerrain,
                       const std::vector<std::vector<rNode *>> &gRoads,
                       const std::shared_ptr<gIGrid<std::list<uint32_t>>> &gLayerOwn,
                       const std::shared_ptr<sCompanyStorage> &sTComp)
            : rIMenu(nullptr, rIMenu::rRelativePos::pBottomRight, ""),
              refPile(rPile), lstValueLayer(1),
              refLRoads(gRoads), rBM_refTerrain(sgTerrain), refLBuild(gLayerOwn), refSComp(sTComp) {
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
                case 2:
                    refPile->inSim->switchActual(gSimLayersTypes::G_UNDERGROUND);
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
                    refPile->rInteractionGameVel = 50.0;
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
                } else if (event.key.code == sf::Keyboard::P) {
                    std::shared_ptr<rGlobalAttrViewLayer> rGlob = std::make_shared<rGlobalAttrViewLayer>(
                            refPile->vTopActiveMenu);
                    refPile->addMenuTop(rGlob);
                }
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed) override {
        std::cout << cPressed.first << ":" << cPressed.second << std::endl;
        switch (rBM_refTerrain->gTG_TypeGen->get(cPressed)) {
            case 1: {
                std::shared_ptr<rHomeViewLayer> _rHome = std::make_shared<rHomeViewLayer>(
                        rHomeViewLayer(refPile->vTopActiveMenu,
                                       refLBuild->get(cPressed).empty() ? nullptr : refSComp->getCompanyByUUID(
                                               refLBuild->get(cPressed).front()),
                                       rBM_refTerrain->gTG_civilOccupancy->get(cPressed),
                                       rBM_refTerrain->getMaxOccByPos(cPressed),
                                       rBM_refTerrain->gTG_TypeSoil->get(cPressed),
                                       rBM_refTerrain->gTG_baseQualityAttr->get(cPressed), refPile));
                refPile->addMenuTop(_rHome);
            }
                break;
            case 2:
            case 3:
            case 4: {
                if (!refLBuild->get(cPressed).empty()) {
                    if (refLBuild->get(cPressed).size() == 1) {
                        std::shared_ptr<rCompViewLayer> rComp = std::make_shared<rCompViewLayer>(
                                rCompViewLayer(refPile->vTopActiveMenu,
                                               *refSComp->getCompanyByUUID(refLBuild->get(cPressed).front()),
                                               "d_mCompViewLayer", refPile));
                        refPile->addMenuTop(rComp);
                    } else {
                        std::shared_ptr<rCellViewMenu> rComp = std::make_shared<rCellViewMenu>(
                                rCellViewMenu(refPile->vTopActiveMenu,
                                              refSComp->getVecCompByUUID(refLBuild->get(cPressed)),
                                              "d_mCellViewLayer", refPile));
                        refPile->addMenuTop(rComp);
                    }
                }
            }
                break;
            case 5:
            case 6:
                if (refLRoads[cPressed.first][cPressed.second] != nullptr) {
                    if (refLRoads[cPressed.first][cPressed.second]->refCompressed->isCrossing()) {
                        std::shared_ptr<rRoadViewMenu> rRoad = std::make_shared<rRoadCrossView>(
                                refPile->vTopActiveMenu, refLRoads[cPressed.first][cPressed.second]->refCompressed,
                                rIMenu::rRelativePos::pBottomLeft);
                        refPile->addMenuTop(rRoad);
                    } else {
                        std::shared_ptr<rRoadViewMenu> rRoad = std::make_shared<rRoadLineView>(
                                refPile->vTopActiveMenu, refLRoads[cPressed.first][cPressed.second]->refCompressed,
                                rIMenu::rRelativePos::pBottomLeft);
                        refPile->addMenuTop(rRoad);
                    }
                }
                break;
        }
    }


private:
    std::shared_ptr<rPileMenus> refPile;
    int lstValueLayer;

    std::vector<std::vector<rNode *>> refLRoads;
    std::shared_ptr<sgTerrain> rBM_refTerrain;
    std::shared_ptr<gIGrid<std::list<uint32_t>>> refLBuild;
    std::shared_ptr<sCompanyStorage> refSComp;
};

#endif //CITYOFWEIRDFISHES_RBASEMENU_H

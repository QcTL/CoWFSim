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
#include "station/rStationViewLayer.h"
#include "products/rProductsListViewLayer.h"

class rBaseMenu : public rIMenu {
public:
    explicit rBaseMenu(const std::shared_ptr<rPileMenus> &inRPile,
                       const std::shared_ptr<groupLand> &inSGLand,
                       const std::shared_ptr<groupEconomy> &inSGEconomy,
                       const std::shared_ptr<sgRoadsMain> &inSGRoads,
                       const std::shared_ptr<sCompanyStorage> &sTComp)
            : rIMenu(nullptr, rIMenu::rRelativePos::pBottomRight, ""),
              refPile(inRPile), lstValueLayer(1), rBM_refRoads(inSGRoads),
              rBM_refLRoads(inSGRoads->gLayerRoads), rBM_refLand(inSGLand),
              rBM_refEconomy(inSGEconomy), rBM_refLBuild(sTComp->sCT_gLayerOwnership),
              rBM_refSComp(sTComp), rBM_actLayer(gSimLayersTypes::G_CITY) {
    }

    void draw(sf::RenderWindow &inRenderWin) override {}

    void setResponse(int inRetValue, uint16_t inLIDSender) override {
        if (inLIDSender == 3) {
            switch (inRetValue) {
                case 0:
                    refPile->inSim->switchActual(gSimLayersTypes::G_AIRPOLLUTION);
                    rBM_actLayer = gSimLayersTypes::G_AIRPOLLUTION;
                    break;
                case 1:
                    refPile->inSim->switchActual(gSimLayersTypes::G_CITY);
                    rBM_actLayer = gSimLayersTypes::G_CITY;
                    break;
                case 2:
                    refPile->inSim->switchActual(gSimLayersTypes::G_UNDERGROUND);
                    rBM_actLayer = gSimLayersTypes::G_UNDERGROUND;
                    break;
                case 3:
                    refPile->inSim->switchActual(gSimLayersTypes::G_TRANSIT);
                    rBM_actLayer = gSimLayersTypes::G_TRANSIT;
                    break;
                default:
                    break;
            }
            lstValueLayer = inRetValue;
        } else if (inLIDSender == 16) {
            switch (inRetValue) {
                case 0:
                    refPile->rInteractionGameVel = 0.0;
                    break;
                case 1:
                    refPile->rInteractionGameVel = 1000.0;
                    break;
                case 2:
                    refPile->rInteractionGameVel = 50.0;
                    break;
                case 3:
                    refPile->rInteractionGameVel = 1.0;
                    break;
                default:
                    break;
            }
        }
        if (inLIDSender < 16)
            refPile->removeTop();
    }

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::M) {
                    std::shared_ptr<rSelOptMenu> _rSom = std::make_shared<rSelOptMenu>(
                            refPile->vTopActiveMenu, lstValueLayer);
                    refPile->addMenuTop(_rSom);
                } else if (event.key.code == sf::Keyboard::P) {
                    std::shared_ptr<rGlobalAttrViewLayer> _rGlob = std::make_shared<rGlobalAttrViewLayer>(
                            refPile->vTopActiveMenu);
                    refPile->addMenuTop(_rGlob);
                }else if(event.key.code == sf::Keyboard::O){
                    std::shared_ptr<rProductsListViewLayer> _rProds = std::make_shared<rProductsListViewLayer>(
                            refPile->vTopActiveMenu, rBM_refEconomy);
                    refPile->addMenuTop(_rProds);
                }
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inUTime) override {
        std::cout << cPressed.first << ":" << cPressed.second << std::endl;
        switch (rBM_actLayer) {
            case G_TRANSIT:
            case G_CITY: {
                switch (rBM_refLand->gL_gTerrain->gTG_TypeGen->get(cPressed)) {
                    case 1: {
                        std::shared_ptr<rHomeViewLayer> _rHome = std::make_shared<rHomeViewLayer>(
                                rHomeViewLayer(refPile->vTopActiveMenu,
                                               rBM_refLBuild->get(cPressed).empty() ? nullptr
                                                                                    : rBM_refSComp->getCompanyByUUID(
                                                       rBM_refLBuild->get(cPressed).front()),
                                               rBM_refLand, cPressed, rBM_refEconomy, refPile));
                        refPile->addMenuTop(_rHome);
                    }
                        break;
                    case 2:
                    case 3:
                    case 4: {
                        if (!rBM_refLBuild->get(cPressed).empty()) {
                            if (rBM_refLBuild->get(cPressed).size() == 1) {
                                std::shared_ptr<rCompViewLayer> rComp = std::make_shared<rCompViewLayer>(
                                        rCompViewLayer(refPile->vTopActiveMenu,
                                                       *rBM_refSComp->getCompanyByUUID(
                                                               rBM_refLBuild->get(cPressed).front()), rBM_refEconomy,
                                                       refPile));
                                refPile->addMenuTop(rComp);
                            } else {
                                std::shared_ptr<rCellViewMenu> rComp = std::make_shared<rCellViewMenu>(
                                        rCellViewMenu(refPile->vTopActiveMenu,
                                                      rBM_refSComp->getVecCompByUUID(rBM_refLBuild->get(cPressed)),
                                                      rBM_refEconomy, refPile));
                                refPile->addMenuTop(rComp);
                            }
                        }
                    }
                        break;
                    case 5:
                    case 6:
                        if (rBM_refLRoads[cPressed.first][cPressed.second] != nullptr) {
                            if (rBM_refLRoads[cPressed.first][cPressed.second]->refCompressed->isCrossing()) {
                                std::shared_ptr<rRoadViewMenu> rRoad = std::make_shared<rRoadCrossView>(
                                        refPile->vTopActiveMenu,
                                        rBM_refLRoads[cPressed.first][cPressed.second]->refCompressed);
                                refPile->addMenuTop(rRoad);
                            } else {
                                std::shared_ptr<rRoadViewMenu> rRoad = std::make_shared<rRoadLineView>(
                                        refPile->vTopActiveMenu,
                                        rBM_refLRoads[cPressed.first][cPressed.second]->refCompressed);
                                refPile->addMenuTop(rRoad);
                            }
                        }
                        break;
                }
            }
                break;
            case G_AIRPOLLUTION:
                break;
            case G_UNDERGROUND: {
                int _uuidStation = rBM_refLand->gL_gUnderground->hasStation(cPressed);
                if (_uuidStation != -1) {
                    std::shared_ptr<rStationViewLayer> _rStation = std::make_shared<rStationViewLayer>(
                            rStationViewLayer(refPile->vTopActiveMenu, rBM_refLand->gL_gUnderground,
                                              _uuidStation, inPTime, inUTime, rBM_refRoads,
                                              "d_mTrainStationLayer", refPile));
                    refPile->addMenuTop(_rStation);
                }
            }
                break;
        }
    }


private:
    std::shared_ptr<rPileMenus> refPile;
    int lstValueLayer;

    std::vector<std::vector<rNode *>> rBM_refLRoads;
    std::shared_ptr<groupLand> rBM_refLand;
    std::shared_ptr<groupEconomy> rBM_refEconomy;
    std::shared_ptr<gIGrid<std::list<uint32_t>>> rBM_refLBuild;
    std::shared_ptr<sCompanyStorage> rBM_refSComp;
    std::shared_ptr<sgRoadsMain> rBM_refRoads;

    gSimLayersTypes rBM_actLayer;
};

#endif //CITYOFWEIRDFISHES_RBASEMENU_H

//
// Created by Laminar on 16/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SGTERRAIN_H
#define CITYOFWEIRDFISHES_SGTERRAIN_H

#include <cstdint>
#include <memory>
#include <list>
#include <random>
#include "../structure/grids/gIGrid.h"
#include "../structure/grids/gBasicGrid.h"
#include "../snCommonAtr.h"
#include "../../display/rgTerrain.h"

class sgTerrain {
public:
    enum sgT_TypeSoil {
        sgT_TS_Nothing = 0,
        sgT_TS_T1Mixed = 1,
        sgT_TS_T2Mixed = 2,
        sgT_TS_T3Mixed = 3,
        sgT_TS_T1Industrial = 4,
        sgT_TS_T2Industrial = 5,
        sgT_TS_T1Farm = 6,
        sgT_TS_T1Protected = 7,
        sgT_TS_T1Obstacle = 8,
        sgT_TS_T2Obstacle = 9,
    };

    enum sgT_TypeGen {
        sgT_TG_Nothing = 0,
        sgT_TG_CivBuilding = 1,
        sgT_TG_IndBuilding = 2,
        sgT_TG_HIndBuilding = 3,
        sgT_TG_FieldBuilding = 4,
        sgT_TG_RoadB = 5,
        sgT_TG_RoadS = 6,
    };

    explicit sgTerrain(uint32_t inGridSize, const std::shared_ptr<sMarketBazaar> &inSMarketBazaar)
            : gTG_marketBazaar(inSMarketBazaar) {
        gTG_TypeSoil = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(inGridSize, inGridSize, 0));
        gTG_TypeGen = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(inGridSize, inGridSize, 0));
        gTG_rLayer = std::make_shared<rgTerrain>(inGridSize);

        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            gTG_genRPos.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            gTG_genRPos.seed(static_cast<unsigned int>(time(nullptr)));
    }


    void setupLists() {
        std::pair<std::pair<int, int>, std::pair<int, int>> _gRange = gTG_TypeSoil->rangeUse();
        for (int i = _gRange.second.first; i < (_gRange.second.second + 1); i++) {
            for (int j = _gRange.first.first; j < (_gRange.first.second + 1); j++) {
                if (gTG_TypeGen->get(i, j) == sgT_TypeGen::sgT_TG_Nothing) {
                    switch (gTG_TypeSoil->get(i, j)) {
                        case sgT_TypeSoil::sgT_TS_T1Mixed:
                        case sgT_TypeSoil::sgT_TS_T2Mixed:
                        case sgT_TypeSoil::sgT_TS_T3Mixed:
                        case sgT_TypeSoil::sgT_TS_T1Industrial:
                        case sgT_TypeSoil::sgT_TS_T2Industrial: {
                            std::shared_ptr<objCompany> gEmptyCompany; //TODO
                            uint16_t quality; //TODO
                            gTG_marketBazaar->addListing(std::make_shared<sLBuyCell::sMOffering>(
                                    sLBuyCell::sMOffering(gEmptyCompany, {i, j}, gTG_TypeSoil->get(i, j), quality,
                                                          2000)));
                        }
                            break;
                        default:
                            break;
                    }
                } else if (gTG_TypeGen->get(i, j) != sgT_TypeGen::sgT_TG_RoadS &&
                           gTG_TypeGen->get(i, j) != sgT_TypeGen::sgT_TG_RoadB) {
                    switch (gTG_TypeSoil->get(i, j)) {
                        case sgT_TypeSoil::sgT_TS_T1Mixed:
                        case sgT_TypeSoil::sgT_TS_T2Mixed:
                        case sgT_TypeSoil::sgT_TS_T3Mixed:
                            gTG_civilFullCell.emplace_back(i, j);
                            break;
                        case sgT_TypeSoil::sgT_TS_T1Industrial:
                        case sgT_TypeSoil::sgT_TS_T2Industrial:
                            gTG_factoryFullCell.emplace_back(i, j);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    void addNewBuilding(sgT_TypeGen inTSNew, const std::pair<int, int> &inGridPos) {
        gTG_TypeSoil->set(inGridPos, inTSNew);
        if (inTSNew == sgT_TypeGen::sgT_TG_CivBuilding)
            gTG_civilFullCell.push_back(inGridPos);
        else if (inTSNew == sgT_TypeGen::sgT_TG_IndBuilding)
            gTG_factoryFullCell.push_back(inGridPos);

        gTG_rLayer->addNewBuildingRender(inGridPos, inTSNew);
    }

    void destroyBuilding(const std::pair<int, int> &inGridPos) {
        gTG_rLayer->addNewBuildingRender(inGridPos, 0);
    }

    void loadUpRender() {
        gTG_rLayer->loadRenderWithSimGrids(gTG_TypeGen, gTG_TypeSoil);
    }

    std::pair<int, int> returnRandomFullCivil() {
        std::uniform_int_distribution<> dist(0, (int) gTG_civilFullCell.size() - 1);
        int _rIndex = dist(gTG_genRPos);

        auto _it = gTG_civilFullCell.begin();
        std::advance(_it, _rIndex % gTG_civilFullCell.size());
        return *_it;
    };

    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeSoil;
    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeGen;
    std::shared_ptr<rgTerrain> gTG_rLayer;

private:
    std::list<std::pair<int, int>> gTG_civilFullCell;
    std::list<std::pair<int, int>> gTG_factoryFullCell;

    std::shared_ptr<sMarketBazaar> gTG_marketBazaar;
    std::mt19937 gTG_genRPos;
};

#endif //CITYOFWEIRDFISHES_SGTERRAIN_H

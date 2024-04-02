//
// Created by Laminar on 16/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SGTERRAIN_H
#define CITYOFWEIRDFISHES_SGTERRAIN_H

#include <cstdint>
#include <memory>
#include <list>
#include <random>
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "../../snCommonAtr.h"
#include "../../../display/rgTerrain.h"

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

    explicit sgTerrain(uint32_t inGridSize) {
        gTG_TypeSoil = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(inGridSize, inGridSize, 0));
        gTG_TypeGen = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(inGridSize, inGridSize, 0));
        gTG_civilOccupancy = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(inGridSize, inGridSize, 0));

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
                        case sgT_TypeSoil::sgT_TS_T2Industrial:
                            gTG_emptyCell.push_back({gTG_TypeSoil->get(i, j), {i, j}});
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
                            gTG_civilPresentCell.emplace_back(i, j);
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

    void addNewBuilding(const std::pair<int, int> &inGridPos) {
        switch (gTG_TypeSoil->get(inGridPos)) {
            case sgT_TypeSoil::sgT_TS_T1Mixed:
            case sgT_TypeSoil::sgT_TS_T2Mixed:
            case sgT_TypeSoil::sgT_TS_T3Mixed:
                gTG_TypeGen->set(inGridPos, sgT_TypeGen::sgT_TG_CivBuilding);
                gTG_civilPresentCell.push_back(inGridPos);

                gTG_rLayer->addNewBuildingRender(inGridPos, sgT_TypeGen::sgT_TG_CivBuilding);
                break;
            case sgT_TypeSoil::sgT_TS_T1Industrial:
            case sgT_TypeSoil::sgT_TS_T2Industrial:
                gTG_TypeGen->set(inGridPos, sgT_TypeGen::sgT_TG_IndBuilding);
                gTG_factoryFullCell.push_back(inGridPos);
                gTG_rLayer->addNewBuildingRender(inGridPos,
                                                 gTG_TypeSoil->get(inGridPos) == sgT_TypeSoil::sgT_TS_T1Industrial
                                                 ? sgT_TypeGen::sgT_TG_IndBuilding : sgT_TypeGen::sgT_TG_HIndBuilding);
                break;
        }
    }

    void removeBuilding(const std::pair<int, int> &inGridPos) {
        switch (gTG_TypeSoil->get(inGridPos)) {
            case sgT_TypeSoil::sgT_TS_T1Mixed:
            case sgT_TypeSoil::sgT_TS_T2Mixed:
            case sgT_TypeSoil::sgT_TS_T3Mixed:
                gTG_civilPresentCell.remove(inGridPos);
                break;
            case sgT_TypeSoil::sgT_TS_T1Industrial:
            case sgT_TypeSoil::sgT_TS_T2Industrial:
                gTG_factoryFullCell.remove(inGridPos);
                break;
        }

        gTG_TypeGen->set(inGridPos, 0);
        gTG_rLayer->addNewBuildingRender(inGridPos, 0);
    }

    void loadUpRender() {
        gTG_rLayer->loadRenderWithSimGrids(gTG_TypeGen, gTG_TypeSoil);
    }

    std::list<std::pair<int, int>>::iterator returnRandomFullCivil() {
        std::uniform_int_distribution<> dist(0, (int) gTG_civilPresentCell.size() - 1);
        int _rIndex = dist(gTG_genRPos);

        auto _it = gTG_civilPresentCell.begin();
        std::advance(_it, _rIndex % gTG_civilPresentCell.size());
        return _it;
    };

    void addCivilHomeToPos(std::list<std::pair<int, int>>::iterator inItPosHome) {
        gTG_civilOccupancy->set(*inItPosHome, gTG_civilOccupancy->get(*inItPosHome) + 1);
        if (gTG_civilOccupancy->get(*inItPosHome) >= getMaxOccBySoil(gTG_TypeSoil->get(*inItPosHome))) {
            gTG_civilPresentCell.erase(inItPosHome);
            gTG_civilFilledCell.push_back(*inItPosHome);
        }
    }

    static uint8_t getMaxOccBySoil(const uint8_t tSoil) {
        switch (tSoil) {
            case sgT_TypeSoil::sgT_TS_T1Mixed:
                return 50;
            case sgT_TypeSoil::sgT_TS_T2Mixed:
                return 25;
            case sgT_TypeSoil::sgT_TS_T3Mixed:
                return 10;
            default:
                return 0;
        }
    }

    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeSoil;
    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeGen;
    std::shared_ptr<rgTerrain> gTG_rLayer;
    std::shared_ptr<gIGrid<uint8_t>> gTG_civilOccupancy;

    struct sgT_emptySlot {
        uint8_t sgT_gType;
        std::pair<int, int> sgT_gPos;
    };

    std::list<sgT_emptySlot> getListPresentCompanies() { return gTG_emptyCell; }

private:
    std::list<std::pair<int, int>> gTG_civilPresentCell;
    std::list<std::pair<int, int>> gTG_civilFilledCell;
    std::list<std::pair<int, int>> gTG_factoryFullCell;
    std::list<sgT_emptySlot> gTG_emptyCell;
    std::mt19937 gTG_genRPos;
};

#endif //CITYOFWEIRDFISHES_SGTERRAIN_H

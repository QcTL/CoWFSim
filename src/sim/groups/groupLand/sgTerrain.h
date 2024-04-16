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
        gTG_baseQualityAttr = std::make_shared<gBasicGrid<uint32_t>>(gBasicGrid<uint32_t>(inGridSize, inGridSize, 20));

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
                        case sgT_TypeSoil::sgT_TS_T1Farm:
                            gTG_emptyCell.push_back({gTG_TypeSoil->get(i, j), {{i, j}}});
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
                        case sgT_TypeSoil::sgT_TS_T1Industrial:
                        case sgT_TypeSoil::sgT_TS_T2Industrial:
                            gTG_fullCell.push_back({gTG_TypeGen->get(i, j), {{i, j}}});
                        case sgT_TypeSoil::sgT_TS_T1Farm: {
                            std::pair<int, int> tPos = {j, i};
                            for (const auto &pair: gTG_fieldsBlobPositions) {
                                if (!pair.second.empty() && pair.second[0].first == tPos) {
                                    std::list<std::pair<int, int>> listValue;
                                    for (const auto &innerPair: pair.second)
                                        listValue.emplace_back(innerPair.first.second, innerPair.first.first);

                                    gTG_fullCell.push_back({gTG_TypeSoil->get(i, j), listValue});
                                    break; // If found, exit the loop
                                }
                            }
                        }
                    }

                    switch (gTG_TypeSoil->get(i, j)) {
                        case sgT_TypeSoil::sgT_TS_T1Mixed:
                        case sgT_TypeSoil::sgT_TS_T2Mixed:
                        case sgT_TypeSoil::sgT_TS_T3Mixed:
                            addToCivilPresentBySoil(gTG_TypeSoil->get(i, j), {i, j});
                            break;
                        case sgT_TypeSoil::sgT_TS_T1Industrial:
                        case sgT_TypeSoil::sgT_TS_T2Industrial:
                        case sgT_TypeSoil::sgT_TS_T1Farm:
                            gTG_factoryFullCell.emplace_back(i, j);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        std::pair<std::pair<int, int>, std::pair<int, int>> range = gTG_baseQualityAttr->rangeUse();
        for (int i = range.second.first; i < (range.second.second + 1); i++) {
            for (int j = range.first.first; j < (range.first.second + 1); j++) {
                switch (gTG_TypeSoil->get(i, j)) {

                    case sgT_TypeSoil::sgT_TS_T1Mixed:
                        gTG_baseQualityAttr->set(i, j, 100);
                        break;
                    case sgT_TypeSoil::sgT_TS_T2Mixed:
                        gTG_baseQualityAttr->set(i, j, 50);
                        break;
                    case sgT_TypeSoil::sgT_TS_T3Mixed:
                        gTG_baseQualityAttr->set(i, j, 35);
                        break;
                }
                if (gPointToNearestElem::find<uint8_t>(gTG_TypeSoil, {i, j}, sgT_TypeSoil::sgT_TS_T1Obstacle,
                                                       10).first != -1)
                    gTG_baseQualityAttr->set(i, j, gTG_baseQualityAttr->get(i, j) + 15);

            }
        }
    }

    void addNewBuilding(const std::pair<int, int> &inGridPos) {
        switch (gTG_TypeSoil->get(inGridPos)) {
            case sgT_TypeSoil::sgT_TS_T1Mixed:
            case sgT_TypeSoil::sgT_TS_T2Mixed:
            case sgT_TypeSoil::sgT_TS_T3Mixed:
                gTG_TypeGen->set(inGridPos, sgT_TypeGen::sgT_TG_CivBuilding);
                addToCivilPresentBySoil(gTG_TypeSoil->get(inGridPos), inGridPos);
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
                gTG_civilPresentCellBySoil[gTG_TypeSoil->get(inGridPos)].remove(inGridPos);
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
        gTG_rLayer->loadRenderWithSimGrids(gTG_TypeGen, gTG_TypeSoil, gTG_fieldsBlobPositions);
    }

    std::pair<int, int> returnRandomFullCivil() {
        std::uniform_int_distribution<> dist(0, (int) gTG_civilPresentCellBySoil.size() - 1);
        auto it = std::next(gTG_civilPresentCellBySoil.begin(), dist(gTG_genRPos));

        const auto &selLists = it->second;
        std::uniform_int_distribution<> dist2(0, (int) selLists.size() - 1);
        return *std::next(selLists.begin(), dist2(gTG_genRPos));
    }

    //TODO A REMOVE CIVILIAN FROM HOUSE POS
    void addCivilHomeToPos(std::pair<int, int> inItPosHome) {
        //TODO REFACTOR
        gTG_civilOccupancy->set(inItPosHome, gTG_civilOccupancy->get(inItPosHome) + 1);

        if (gTG_civilOccupancy->get(inItPosHome) >= getMaxOccBySoil(gTG_TypeSoil->get(inItPosHome))) {
            gTG_civilPresentCellBySoil[gTG_TypeSoil->get(inItPosHome)].remove(inItPosHome);
            if (gTG_civilFilledCellBySoil.find(gTG_TypeSoil->get(inItPosHome)) == gTG_civilFilledCellBySoil.end())
                gTG_civilFilledCellBySoil[gTG_TypeSoil->get(inItPosHome)] = {};
            gTG_civilFilledCellBySoil[gTG_TypeSoil->get(inItPosHome)].push_back(inItPosHome);
        }
    }

    [[nodiscard]] uint8_t getMaxOccByPos(const std::pair<int, int> &inPCell) const {
        return getMaxOccBySoil(gTG_TypeSoil->get(inPCell));
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


    [[nodiscard]] std::pair<int, int>
    getHomeSortLessQuality(const uint8_t inTypeSoil) {
        //return gTG_civilPresentCellBySoil[inTypeSoil].front();
        std::vector<std::pair<int, int>> topValidHomes(gTG_civilPresentCellBySoil[inTypeSoil].begin(),
                                                       std::next(gTG_civilPresentCellBySoil[inTypeSoil].begin(), 5));
        std::shuffle(topValidHomes.begin(), topValidHomes.end(), gTG_genRPos);
        return topValidHomes.front();
    }

    [[nodiscard]] uint32_t getQualityGivenPosHome(const std::pair<int, int> inPHouse) const {
        return gTG_baseQualityAttr->get(inPHouse) * 3;
    }

    [[nodiscard]]  double getRemainHomes(const uint8_t inTypeSoil) const {
        double gTotalBuild = (double) gTG_civilPresentCellBySoil.at(inTypeSoil).size() +
                             (gTG_civilFilledCellBySoil.find(inTypeSoil) != gTG_civilFilledCellBySoil.end()
                              ? (double) gTG_civilFilledCellBySoil.at(inTypeSoil).size() : 0);
        return (double) gTG_civilPresentCellBySoil.at(inTypeSoil).size() / gTotalBuild;
    }

    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeSoil;
    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeGen;
    std::shared_ptr<rgTerrain> gTG_rLayer;
    std::shared_ptr<gIGrid<uint8_t>> gTG_civilOccupancy;
    std::shared_ptr<gIGrid<uint32_t>> gTG_baseQualityAttr;
    std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> gTG_fieldsBlobPositions;

    struct sgT_CellSlot {
        uint8_t sgT_gType;
        std::list<std::pair<int, int>> sgT_gPos;
    };

    std::list<sgT_CellSlot> getListPresentCompanies() { return gTG_fullCell; }

    std::list<sgT_CellSlot> getListEmptyCompanies() { return gTG_emptyCell; }

private:

    void addToCivilPresentBySoil(const uint8_t inKey, std::pair<int, int> intPosToAdd) {
        if (gTG_civilPresentCellBySoil.find(gTG_TypeSoil->get(intPosToAdd)) == gTG_civilPresentCellBySoil.end())
            gTG_civilPresentCellBySoil[inKey] = {};
        auto insertPos = std::find_if(gTG_civilPresentCellBySoil[inKey].begin(),
                                      gTG_civilPresentCellBySoil[gTG_TypeSoil->get(intPosToAdd)].end(),
                                      [&](const std::pair<int, int> element) {
                                          return gTG_baseQualityAttr->get(element) <
                                                 gTG_baseQualityAttr->get(intPosToAdd);
                                      });
        gTG_civilPresentCellBySoil[inKey].insert(insertPos, intPosToAdd);
    }

    std::map<uint8_t, std::list<std::pair<int, int>>> gTG_civilPresentCellBySoil;
    std::map<uint8_t, std::list<std::pair<int, int>>> gTG_civilFilledCellBySoil;
    std::list<std::pair<int, int>> gTG_factoryFullCell;
    std::list<sgT_CellSlot> gTG_emptyCell;
    std::list<sgT_CellSlot> gTG_fullCell;
    std::mt19937 gTG_genRPos;
};

#endif //CITYOFWEIRDFISHES_SGTERRAIN_H

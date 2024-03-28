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

        std::srand(static_cast<unsigned int>(std::time(nullptr)));
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
                            gTG_civilEmptyCell.emplace_back(i, j);
                            break;
                        case sgT_TypeSoil::sgT_TS_T1Industrial:
                        case sgT_TypeSoil::sgT_TS_T2Industrial:
                            gTG_factoryEmptyCell.emplace_back(i, j);
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

    void changeStateSoil(sgT_TypeSoil inTSNew, const std::pair<int, int> &inGridPos) {
        gTG_TypeSoil->set(inGridPos, inTSNew);
        if (inTSNew == sgT_TypeSoil::sgT_TS_T1Mixed || inTSNew == sgT_TypeSoil::sgT_TS_T2Mixed ||
            inTSNew == sgT_TypeSoil::sgT_TS_T3Mixed)
            gTG_civilEmptyCell.push_back(inGridPos);
        else if (inTSNew == sgT_TypeSoil::sgT_TS_T1Industrial || inTSNew == sgT_TypeSoil::sgT_TS_T2Industrial)
            gTG_factoryEmptyCell.push_back(inGridPos);
    }

    std::list<std::pair<int, int>>::iterator getEmptyPositionByType(const sgT_TypeGen inTypeGen) {
        std::mt19937 gen;
        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            gen.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            gen.seed(static_cast<unsigned int>(time(nullptr)));

        std::list<std::pair<int, int>>::iterator _it;
        uint32_t _rIndex = 0;

        switch (inTypeGen) {
            case sgT_TypeGen::sgT_TG_CivBuilding: {
                std::uniform_int_distribution<> dist(0, (int) gTG_civilEmptyCell.size() - 1);
                _rIndex = dist(gen);
                _it = gTG_civilEmptyCell.begin();
            }
                break;
            case sgT_TypeGen::sgT_TG_IndBuilding:
            case sgT_TypeGen::sgT_TG_HIndBuilding: {
                std::uniform_int_distribution<> dist(0, (int) gTG_factoryEmptyCell.size() - 1);
                _rIndex = dist(gen);
                _it = gTG_factoryEmptyCell.begin();
            }
                break;
            default:
                break;
        }
        std::advance(_it, _rIndex);
        return _it;
    }

    void
    removeEmptyPositionByIterator(const sgT_TypeGen inTypeSoil,
                                  const std::list<std::pair<int, int>>::iterator &inItToRemove) {
        switch (inTypeSoil) {
            case sgT_TypeGen::sgT_TG_CivBuilding:
                gTG_TypeGen->set(*inItToRemove, 1);
                gTG_civilEmptyCell.erase(inItToRemove);
                break;
            case sgT_TypeGen::sgT_TG_IndBuilding:
            case sgT_TypeGen::sgT_TG_HIndBuilding:
                gTG_TypeGen->set(*inItToRemove, inTypeSoil);
                gTG_factoryEmptyCell.erase(inItToRemove);
                break;
            default:
                break;
        }
    }

    std::pair<int, int> returnRandomFullCivil() {
        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
        else
            std::srand(snCommonAtr::getFlagAtr("snCA_Seed"));
        auto _it = gTG_civilFullCell.begin();
        std::advance(_it, std::rand() % gTG_civilFullCell.size());
        return *_it;
    };

    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeSoil;
    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeGen;

private:
    std::list<std::pair<int, int>> gTG_civilEmptyCell;
    std::list<std::pair<int, int>> gTG_civilFullCell;
    std::list<std::pair<int, int>> gTG_factoryEmptyCell;
    std::list<std::pair<int, int>> gTG_factoryFullCell;
};

#endif //CITYOFWEIRDFISHES_SGTERRAIN_H

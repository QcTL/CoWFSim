//
// Created by Laminar on 16/03/2024.
//

#ifndef CITYOFWEIRDFISHES_GTERRAINGRID_H
#define CITYOFWEIRDFISHES_GTERRAINGRID_H

#include <cstdint>
#include <memory>
#include <list>
#include <random>
#include "../structure/grids/gIGrid.h"
#include "../structure/grids/gBasicGrid.h"

class gTerrainGrid {
public:
    gTerrainGrid(uint32_t lSize) {

        gTG_TypeSoil = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));
        gTG_TypeGen = std::make_shared<gBasicGrid<uint8_t>>(gBasicGrid<uint8_t>(lSize, lSize, 0));

        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gTG_TypeSoil->rangeUse();
        for (int i = gRange.second.first; i < (gRange.second.second + 1); i++) {
            for (int j = gRange.first.first; j < (gRange.first.second + 1); j++) {
                if (gTG_TypeGen->get(i, j) == 0)
                    if (gTG_TypeSoil->get(i, j) == 1 || gTG_TypeSoil->get(i, j) == 2 || gTG_TypeSoil->get(i, j) == 3)
                        gTG_CivilEmptyCell.emplace_back(i, j);
                    else if (gTG_TypeSoil->get(i, j) == 4 || gTG_TypeSoil->get(i, j) == 5)
                        gTG_FactoryEmptyCell.emplace_back(i, j);
            }
        }
    }


    void changeStateSoil(uint8_t newType, std::pair<int, int> gPos) {
        gTG_TypeSoil->set(gPos, newType);
        if (newType == 1 || newType == 2 || newType == 3)
            gTG_CivilEmptyCell.push_back(gPos);
        else if (newType == 4 || newType == 5)
            gTG_FactoryEmptyCell.push_back(gPos);
    }


    std::list<std::pair<int, int>>::iterator getEmptyPositionByType(const uint8_t gType) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::list<std::pair<int, int>>::iterator it;
        uint32_t rIndex = 0;

        if (gType == 1 || gType == 2 || gType == 3) {
            std::uniform_int_distribution<> dist(0, gTG_CivilEmptyCell.size() - 1);
            rIndex = dist(gen);
            it = gTG_CivilEmptyCell.begin();
        } else if (gType == 4 || gType == 5) {
            std::uniform_int_distribution<> dist(0, gTG_FactoryEmptyCell.size() - 1);
            rIndex = dist(gen);
            it = gTG_FactoryEmptyCell.begin();
        }
        std::advance(it, rIndex);
        return it;
    }

    void
    removeEmptyPositionByIterator(const uint8_t gType, const std::list<std::pair<int, int>>::iterator &itToRemove) {
        if (gType == 1 || gType == 2 || gType == 3) {
            gTG_TypeGen->set(*itToRemove, 1);
            gTG_CivilEmptyCell.erase(itToRemove);
        }
        else if (gType == 4 || gType == 5) {
            gTG_TypeGen->set(*itToRemove, 3); // TODO IT CAN ALSO BE TWO
            gTG_FactoryEmptyCell.erase(itToRemove);
        }
    }

    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeSoil;
    // NOTHING
    // TYPE 1 Mixed
    // TYPE 2 Mixed
    // TYPE 3 Mixed
    // TYPE 1 Industrial
    // TYPE 2 Industrial
    // TYPE 1 Farm
    // TYPE 1 Protected
    // TYPE 1 Obstacle, can be circumvented
    // TYPE 2 Obstacle, solid

    std::shared_ptr<gIGrid<uint8_t>> gTG_TypeGen;
    //VAL 0: NOTHING;
    //VAL 1: CIVIL BUILDING;
    //VAL 2: FACTORY BUILDING;
    //VAL 3: HEAVY FACTORY BUILDING;
    //VAL 4: FIELDS;
    //VAL 5: ROADS BIG;
    //VAL 6: ROADS SMALL;
private:
    std::list<std::pair<int, int>> gTG_CivilEmptyCell;
    std::list<std::pair<int, int>> gTG_FactoryEmptyCell;
};

#endif //CITYOFWEIRDFISHES_GTERRAINGRID_H

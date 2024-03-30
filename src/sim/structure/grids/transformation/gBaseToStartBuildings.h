//
// Created by Laminar on 03/03/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOSTARTBUILDINGS_H
#define CITYOFWEIRDFISHES_GBASETOSTARTBUILDINGS_H

#include <cstdint>
#include <memory>
#include <vector>
#include <set>
#include "../gIGrid.h"
#include "../../../layers/implementations/sLayerType.h"

class gBaseToStartBuildings {
public:

    static std::vector<retObjCompany>
    gen(const std::shared_ptr<gIGrid<uint8_t>> &tSoil, const std::shared_ptr<gIGrid<uint8_t>> &tType,
        const std::vector<uint8_t> &groupSoil, const std::vector<uint8_t> &groupRoads,
        int seed = 0) {

        std::vector<retObjCompany> sFactory;
        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = tType->rangeUse();
        std::set<uint8_t> sGroupSoil(groupSoil.begin(), groupSoil.end());
        std::set<uint8_t> sGroupRoads(groupRoads.begin(), groupRoads.end());
        if (seed == 0)
            srand(static_cast<unsigned>(time(nullptr)));
        else
            srand(seed);

        for (int i = gRange.first.first; i <= gRange.first.second; ++i) {
            for (int j = gRange.second.first; j <= gRange.second.second; ++j) {
                if (sGroupSoil.find(tSoil->get(i, j)) != sGroupSoil.end() && tType->get(i, j) == 0 &&
                    rand() % (tSoil->get(i, j)) == 0) {
                    switch (tSoil->get(i, j)) {
                        case TypeSoil_T1Urban:
                            tType->set(i, j, 1);
                            sFactory.push_back(retObjCompany({{i, j}}, 1));
                            break;
                        case TypeSoil_T2Urban:
                            tType->set(i, j, 1);
                            break;
                        case TypeSoil_T3Urban:
                            tType->set(i, j, 1);
                            break;
                        case TypeSoil_T1Factory:
                            tType->set(i, j, 3);
                            sFactory.push_back(retObjCompany({{i, j}}, 3));
                            break;
                        case TypeSoil_T2Factory:
                            tType->set(i, j, 2);
                            sFactory.push_back(retObjCompany({{i, j}}, 3));
                            break;
                    }
                }
            }
        }
        return sFactory;
    }

    template<typename T>
    static void
    renderBuildingFromType(const std::shared_ptr<gIGrid<T>> &tRender, const std::shared_ptr<gIGrid<uint8_t>> &tType,
                           const std::shared_ptr<gIGrid<uint8_t>> &tSoil,
                           int seed = 0) {

        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = tType->rangeUse();
        if (seed == 0)
            srand(static_cast<unsigned>(time(nullptr)));
        else
            srand(seed);

        for (int i = gRange.first.first; i <= gRange.first.second; ++i) {
            for (int j = gRange.second.first; j <= gRange.second.second; ++j) {
                if (tType->get(i, j) != 5 && tType->get(i, j) != 6 && tType->get(i, j) != 0) {
                    switch (tSoil->get(i, j)) {
                        case TypeSoil_T1Urban:
                            tRender->set(i, j, (((uint32_t) (uint8_t) strtol("00010000", nullptr, 2)) << 24) +
                                               rand() % (3 + 1));
                            break;
                        case TypeSoil_T2Urban:
                            tRender->set(i, j, (((uint32_t) (uint8_t) strtol("00010001", nullptr, 2)) << 24) +
                                               rand() % (2 + 1));
                            break;
                        case TypeSoil_T3Urban:
                            tRender->set(i, j, (((uint32_t) (uint8_t) strtol("00010010", nullptr, 2)) << 24) +
                                               rand() % (3 + 1));
                            break;
                        case TypeSoil_T1Factory:
                            tRender->set(i, j, (((uint32_t) (uint8_t) strtol("00010101", nullptr, 2)) << 24) +
                                               rand() % (2 + 1));
                            break;
                        case TypeSoil_T2Factory:
                            tRender->set(i, j, (((uint32_t) (uint8_t) strtol("00010101", nullptr, 2)) << 24) +
                                               rand() % (2 + 1));
                            break;
                    }
                }
            }
        }
    }
};

#endif //CITYOFWEIRDFISHES_GBASETOSTARTBUILDINGS_H

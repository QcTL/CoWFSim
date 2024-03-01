//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOFIELD_H
#define CITYOFWEIRDFISHES_GBASETOFIELD_H

#include <memory>
#include <random>
#include <list>

#include "../gIGrid.h"
#include "cEspec/gHelpBlobbing.h"
#include "gBaseToBorderDetection.h"

template<typename T>
class gBaseToField {
public:
    explicit gBaseToField(std::shared_ptr<gIGrid<T>> gR, T endValue,
                          const std::shared_ptr<gIGrid<bool>> &pMask = nullptr, int seed = -1)
            : hasMask(pMask != nullptr), gMask(pMask) {
        gen.seed(seed);
        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gR->rangeUse();
        int GWidth = (gRange.first.second - gRange.first.first) + 1;
        int GHeight = (gRange.second.second - gRange.second.first) + 1;

        std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>> g =
                gHelpBlobbing::genListOfUniquePos(
                        gHelpBlobbing::genGridUnique(GHeight, GWidth));

        dis_row = std::uniform_int_distribution<>(0, GWidth - 1);
        dis_col = std::uniform_int_distribution<>(0, GHeight - 1);
        dis_dir = std::uniform_int_distribution<>(0, 3);

        for (int i = 0; i < 5000; i++) {
            g = gHelpBlobbing::blobTwoPositions(g, gen, dis_row, dis_col, dis_dir);
        }

        std::shared_ptr<gIGrid<T>> gGrid = gHelpBlobbing::blobToGrid<T>(g);
        auto p = gBaseToBorderDetection::generate(gGrid,
                                                  {gBorderType::gBNonConnex, gBorderOutside::gIsNotGroup}, {});
        blobFieldsToGrid(p, gR);
    }

private:
    bool hasMask;
    std::shared_ptr<gIGrid<bool>> gMask;

    std::mt19937 gen;
    std::uniform_int_distribution<> dis_row;
    std::uniform_int_distribution<> dis_col;
    std::uniform_int_distribution<> dis_dir;

    void blobFieldsToGrid(std::map<T, std::vector<std::pair<std::pair<int, int>, uint8_t>>> edges,
                          std::shared_ptr<gIGrid<T>> gFinal) {
        for (const auto &mapElem: edges) {
            //if (mapElem.second.size() >= 5) {
                for (const auto &vecElem: mapElem.second) {
                    std::pair<int, int> pair = vecElem.first;
                    if (gFinal->isInside({pair.second, pair.first}) &&
                        (!hasMask || gMask->get({pair.second, pair.first}))) {
                        uint8_t p = ((vecElem.second & (1 << 1)) != 0) << 3
                                    | ((vecElem.second & (1 << 4)) != 0) << 2
                                    | ((vecElem.second & (1 << 6)) != 0) << 1
                                    | ((vecElem.second & (1 << 3)) != 0);
                        gFinal->set({vecElem.first.second, vecElem.first.first},
                                    (((uint32_t) (uint8_t) strtol("1000000", nullptr, 2)) << 24) + p);
                    }
                }
           // }
        }
    }

};

#endif //CITYOFWEIRDFISHES_GBASETOFIELD_H

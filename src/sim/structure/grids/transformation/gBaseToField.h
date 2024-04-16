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
    [[nodiscard]]
    static std::map<uint32_t , std::vector<std::pair<std::pair<int, int>, uint8_t>>>
    genMapPos(std::shared_ptr<gIGrid<T>> gR,
              const std::shared_ptr<gIGrid<bool>> &pMask = nullptr, int seed = -1) {
        bool hasMask = pMask != nullptr;

        std::mt19937 gen(seed);
        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gR->rangeUse();
        int GWidth = (gRange.first.second - gRange.first.first) + 1;
        int GHeight = (gRange.second.second - gRange.second.first) + 1;

        std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>> g =
                gHelpBlobbing::genListOfUniquePos(
                        gHelpBlobbing::genGridUnique(GHeight, GWidth));

        std::uniform_int_distribution<> dis_row = std::uniform_int_distribution<>(0, GWidth - 1);
        std::uniform_int_distribution<> dis_col = std::uniform_int_distribution<>(0, GHeight - 1);
        std::uniform_int_distribution<> dis_dir = std::uniform_int_distribution<>(0, 3);

        for (int i = 0; i < 5000; i++) {
            g = gHelpBlobbing::blobTwoPositions(g, gen, dis_row, dis_col, dis_dir);
        }

        std::shared_ptr<gIGrid<uint32_t>> gGrid = gHelpBlobbing::blobToGrid<uint32_t>(g);
        std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> mGenFields = gBaseToBorderDetection::generate(
                gGrid, {gBorderType::gBNonConnex, gBorderOutside::gIsNotGroup}, {});

        for (auto it = mGenFields.begin(); it != mGenFields.end();) {
            bool hasAllInsideMask = true;
            for (const auto &vecElem: it->second) {
                std::pair<int, int> pair = vecElem.first;
                bool complyMask = !hasMask || pMask->get(pair.second, pair.first);
                bool complyGrid =  gR->isInside(pair.second, pair.first);
                bool isUsed = gR->get(pair.second, pair.first) == 5 || gR->get(pair.second, pair.first) == 6;
                if (!complyMask || !complyGrid || isUsed) {
                    hasAllInsideMask = false;
                    break;
                }
            }
            if (!hasAllInsideMask)
                it = mGenFields.erase(it); // Remove the element from the map
            else
                ++it;

        }
        return mGenFields;
    }

    static void blobFieldsToGrid(const std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> &edges,
                                 std::shared_ptr<gIGrid<T>> gFinal) {
        for (const auto &mapElem: edges) {
            for (const auto &vecElem: mapElem.second) {
                uint8_t p = ((vecElem.second & (1 << 1)) != 0) << 3
                            | ((vecElem.second & (1 << 4)) != 0) << 2
                            | ((vecElem.second & (1 << 6)) != 0) << 1
                            | ((vecElem.second & (1 << 3)) != 0);
                gFinal->set({vecElem.first.second, vecElem.first.first},
                            (((uint32_t) (uint8_t) strtol("1000000", nullptr, 2)) << 24) + p);
            }
        }
    }

};

#endif //CITYOFWEIRDFISHES_GBASETOFIELD_H

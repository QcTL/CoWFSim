//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SGRIDTOSIMULATOR_H
#define CITYOFWEIRDFISHES_SGRIDTOSIMULATOR_H

#include <memory>

#include "../gIGrid.h"
#include "../../../sMainSimulator.h"

class sGridToSimulator {

public:

    static void gToTypeCell(std::shared_ptr<gIGrid<std::pair<uint8_t, std::pair<uint8_t, uint32_t>>>> &gSim,
                            const std::shared_ptr<gIGrid<uint32_t>> &gOrigin) {
        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gOrigin->rangeUse();

        for (int i = gRange.second.first; i <= gRange.second.second; i++) {
            for (int j = gRange.first.first; j <= gRange.first.second; j++) {
                uint8_t nTypeCell;

                switch (gOrigin->get(i, j)) {
                    case 2:
                        nTypeCell = 0;
                        break;
                    case 1:
                        nTypeCell = 1;
                        break;
                    case 0:
                        nTypeCell = 4;
                        break;
                }
                gSim->set(i, j, {nTypeCell, {0,0}});
            }
        }
    }

    static void gToCurStruct(const std::shared_ptr<gIGrid<uint32_t>> &gSim,
                             const std::shared_ptr<gIGrid<uint8_t>> &gType,
                             const std::shared_ptr<gIGrid<uint32_t>> &gOrigin) {
        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gOrigin->rangeUse();
        for (int i = gRange.second.first; i <= gRange.second.second; i++) {
            for (int j = gRange.first.first; j <= gRange.first.second; j++) {
                gSim->set(i, j, gOrigin->get(i, j));
                switch (static_cast<uint8_t>(gOrigin->get(i, j) >> 24)) {
                    case 0:
                        gType->set(i, j, 0);
                        break;
                    case 48:
                        gType->set(i, j, 3);
                        break;
                    case 32:
                    case 33:
                        gType->set(i, j, 2);
                        break;
                    default:
                        gType->set(i, j, 1);
                        break;
                }
            }
        }
    }

};

#endif //CITYOFWEIRDFISHES_SGRIDTOSIMULATOR_H

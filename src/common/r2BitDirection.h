//
// Created by Laminar on 12/03/2024.
//

#ifndef CITYOFWEIRDFISHES_R2BITDIRECTION_H
#define CITYOFWEIRDFISHES_R2BITDIRECTION_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include "RelPath.h"
#include "gCSVLoader.h"

class r2BitDirection {
public:
    static void loadPossibleRoads(const std::string &gPathRoads) {
        gVect = gCSVLoader::givenPath(gPathRoads, ',');

        uint32_t tCounterR1 = 0;
        uint32_t tCounterR2 = 0;
        for (const std::vector<std::string> &g: gVect) {
            uint8_t gRef = 0;
            for (size_t i = 0; i < g[1].size(); ++i) {
                uint8_t rTot;
                if (g[1][i] == '1')
                    rTot = 1;
                else if (g[1][i] == '2')
                    rTot = 2;
                else
                    rTot = 0;
                gRef |= rTot << i * 2;
            }

            gRefByAdjacent[gRef] = {static_cast<uint32_t>(std::stoul(g[0])), g[0] == "1" ? tCounterR1 : tCounterR2};

            if (g[0] == "1")
                tCounterR1++;
            else if (g[0] == "2")
                tCounterR2++;
        }
    }

    static int gTotalRoads() {
        return gRefByAdjacent.size();
    }

    static uint32_t getTypeRoadById(const uint8_t &gId) {
        return static_cast<uint32_t>(std::stoul(gVect[gId][0]));
    }

    static std::map<uint8_t, std::pair<uint32_t, uint32_t>> gRefByAdjacent;
    static std::vector<std::vector<std::string>> gVect;

    static uint32_t getIdBy8Code(uint8_t idCode, uint8_t cCode) {
        for (int i = 0; i < 4; i++) {
            uint8_t nRet = (idCode >> 2 * i) | (idCode << (8 - 2 * i));
            if (gRefByAdjacent.find(nRet) != gRefByAdjacent.end() && gRefByAdjacent[nRet].first == cCode)
                return gRefByAdjacent[nRet].second << 2 | i;
        }

        uint8_t pGeneric = ((idCode & (3 << 6)) != 0) * cCode  << 6
                    | ((idCode & (3 << 4)) != 0)* cCode << 4
                    | ((idCode & (3 << 2)) != 0)* cCode << 2
                    | ((idCode & (3 << 0)) != 0);
        for (int i = 0; i < 4; i++) {
            uint8_t nRet = (pGeneric >> 2 * i) | (pGeneric << (8 - 2 * i));
            if (gRefByAdjacent.find(nRet) != gRefByAdjacent.end() && gRefByAdjacent[nRet].first == cCode)
                return gRefByAdjacent[nRet].second << 2 | i;
        }
        return 0;
    }
};

inline std::map<uint8_t, std::pair<uint32_t, uint32_t>> r2BitDirection::gRefByAdjacent;
inline std::vector<std::vector<std::string>> r2BitDirection::gVect;
#endif //CITYOFWEIRDFISHES_R2BITDIRECTION_H

//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_STOTALRECIPES_H
#define CITYOFWEIRDFISHES_STOTALRECIPES_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "../../../../common/RelPath.h"
#include "../../../structure/obj/sCommon.h"

class sTotalRecipes {
public:
    explicit sTotalRecipes(const std::string &gPathCSV) {
        std::ifstream fCVS((RelPath::relPath / "files" / "objCollectives" / gPathCSV).string());
        if (!fCVS.is_open())
            std::cerr << "Error opening file." << std::endl;

        uint32_t idAct = 0;
        while (!fCVS.eof()) {
            gRecipeTotal.push_back(getRecipeFromTokens(idAct, splitTokens(fCVS, ',')));
            idAct++;
        }
        fCVS.close();
    }

    objProdRecipe getById(uint32_t idRec) { return gRecipeTotal[idRec]; }

private:

    std::vector<objProdRecipe> gRecipeTotal;
    std::map<std::string, uint32_t> gRecipeIdByName;

    objProdRecipe getRecipeFromTokens(const uint32_t &idAct, const std::vector<std::string> &gTokens) {
        std::vector<uint16_t> rTypeBuilding;

        for (const std::string &c: split(gTokens[1], '.')) {
            if (c == "GTYPE_FARMLAND")
                rTypeBuilding.push_back(0);
            else if (c == "GTYPE_LOWFACT")
                rTypeBuilding.push_back(1);
            else if (c == "GTYPE_HEAVYFACT")
                rTypeBuilding.push_back(2);
        }

        std::vector<uint32_t> rReqMaterials;
        for (const std::string &c: split(gTokens[2], '.'))
            rReqMaterials.push_back(gRecipeIdByName.at(c));

        gRecipeIdByName.insert({gTokens[0], idAct});

        return {idAct, rTypeBuilding, rReqMaterials, static_cast<uint32_t>(std::stoul(gTokens[3]))};
    }

    static std::vector<std::string> split(const std::string &s, char delimiter) {
        std::vector<std::string> splits;
        std::string split;
        std::istringstream ss(s);

        while (std::getline(ss, split, delimiter)) {
            splits.push_back(split);
        }
        return splits;
    }

    static std::vector<std::string> splitTokens(std::istream &strLine, const char gSep) {
        std::string line;
        std::getline(strLine, line);
        return split(line, gSep);
    }

};

#endif //CITYOFWEIRDFISHES_STOTALRECIPES_H

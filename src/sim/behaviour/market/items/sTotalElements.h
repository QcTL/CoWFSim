//
// Created by ganymede on 3/22/24.
//

#ifndef CITYOFWEIRDFISHES_STOTALELEMENTS_H
#define CITYOFWEIRDFISHES_STOTALELEMENTS_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "../../../../common/RelPath.h"

class sTotalElements {
public:
    struct sME_Element {
        uint64_t sMEE_uuid;

        std::vector<uint64_t> sMEE_iCElem;
        std::uint8_t sMEE_iReqTypeBuild;
        std::uint32_t sMEE_iTime;

        [[nodiscard]] uint32_t getPrice(const double desirability, const std::vector<uint32_t> &vDatePriceElems) const {
            if (sMEE_iCElem.empty())
                return (uint32_t) (vDatePriceElems[sMEE_uuid] * desirability);
            else {
                uint32_t vRet = 0;
                for (const uint64_t uuidCred: sMEE_iCElem) {
                    vRet += vDatePriceElems[uuidCred];
                }
                return (uint32_t) ((vRet + vDatePriceElems[sMEE_uuid]) * desirability);
            }
        }
    };

    explicit sTotalElements(const std::string &gPathCSV) {
        std::ifstream fCVS((RelPath::relPath / "files" / "objCollectives" / gPathCSV).string());
        if (!fCVS.is_open())
            std::cerr << "Error opening file." << std::endl;

        uint32_t idAct = 0;
        while (!fCVS.eof()) {
            sME_listItems.push_back(getRecipeFromTokens(idAct, splitTokens(fCVS, ',')));
            idAct++;
        }
        fCVS.close();
    }

    sME_Element getById(uint32_t idRec) { return sME_listItems[idRec]; }

    uint32_t nElements() { return sME_listItems.size(); }

private:

    std::vector<sME_Element> sME_listItems;
    std::map<std::string, uint32_t> sME_gRecipeIdByName;

    sME_Element getRecipeFromTokens(const uint32_t &idAct, const std::vector<std::string> &gTokens) {
        std::vector<uint8_t> rTypeBuilding;

        for (const std::string &c: split(gTokens[1], '.')) {
            if (c == "GTYPE_FARMLAND")
                rTypeBuilding.push_back(sgTerrain::sgT_TypeSoil::sgT_TS_T1Farm);
            else if (c == "GTYPE_LOWFACT")
                rTypeBuilding.push_back(sgTerrain::sgT_TypeSoil::sgT_TS_T1Industrial);
            else if (c == "GTYPE_HEAVYFACT")
                rTypeBuilding.push_back(sgTerrain::sgT_TypeSoil::sgT_TS_T2Industrial);
            else if (c == "GTYPE_CIV")
                rTypeBuilding.push_back(sgTerrain::sgT_TypeSoil::sgT_TS_T1Mixed);
        }

        std::vector<uint64_t> rReqMaterials;
        for (const std::string &c: split(gTokens[2], '.'))
            rReqMaterials.push_back(sME_gRecipeIdByName.at(c));

        sME_gRecipeIdByName.insert({gTokens[0], idAct});

        return {idAct, rReqMaterials, rTypeBuilding[0] , static_cast<uint32_t>(std::stoul(gTokens[3]))};
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

#endif //CITYOFWEIRDFISHES_STOTALELEMENTS_H

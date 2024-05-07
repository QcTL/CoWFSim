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

/**
 * @class sTotalElements
 * @brief This class is responsible to contain and manage the absolute extracted values of the csv for the products.
 */
class sTotalElements {
public:
    struct sME_Element {
        uint64_t sMEE_uuid;

        std::vector<uint64_t> sMEE_iCElem;
        std::uint8_t sMEE_iReqTypeBuild;
        std::uint32_t sMEE_iTime;

        [[nodiscard]] uint32_t getPrice(const double desirability, const uint32_t inBasicPrice) const {
            return (uint32_t) (inBasicPrice + (desiredFunc(desirability)) * 5);
        }

        static double desiredFunc(const double x) {
            return (1 / (1 + exp(-(x - 1)))) - 0.5;
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


    /**
     * @fn  std::vector<uint64_t> extractUuids
     * @brief Extracts all the uuid of the products described on the csv
     * @param gPathCSV A vector of the uuid of all of the products extracted from the csv
     */
    std::vector<uint64_t> extractUuids() {
        std::vector<uint64_t> uuids;
        uuids.reserve(sME_listItems.size());

        auto extractUuid = [](const sME_Element &element) {
            return element.sMEE_uuid;
        };

        std::transform(sME_listItems.begin(), sME_listItems.end(), std::back_inserter(uuids), extractUuid);
        return uuids;
    }

    /**
     * @fn sME_Element getById
     * @param idRec The uuid of a valid product you want to collect, < sME_listItems.size
     * @return A copy of the object and its attributes referenced by the uuid
     */
    sME_Element getById(uint32_t idRec) { return sME_listItems[idRec]; }

    /**
     * @fn std::string getNameById
     * @param idRec The uuid of a valid product you want to collect, < sME_listItems.size
     * @return The name in form of string of that product
     */
    std::string getNameById(uint32_t idRec) { return sME_gRecipeTotalName[idRec]; }

    uint32_t nElements() { return sME_listItems.size(); }

private:

    std::vector<sME_Element> sME_listItems;
    std::map<std::string, uint32_t> sME_gRecipeIdByName;
    std::vector<std::string> sME_gRecipeTotalName;

    /**
     * @fn sME_Element getRecipeFromTokens
     * @brief Given a line of string, representing a line of the csv, give the corresponding element created
     * @param idAct The uuid that the product will have, has to be unique and incrementing for each call of the function
     * @param gTokens The vector of strings as the line of the csv file
     * @return The struct representing the element that was extracted from that line in the csv
     */
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
        sME_gRecipeTotalName.push_back(gTokens[4]);

        return {idAct, rReqMaterials, rTypeBuilding[0], static_cast<uint32_t>(std::stoul(gTokens[3]))};
    }

    /**
     * @fn std::vector<std::string> split
     * @brief given a string and a delimiter cut the string into the corresponding values as interpreted in a CSV
     * @param s The string containing a repetition of some delimiter
     * @param delimiter The delimiter the csv uses
     * @return A vector of the sliced string
     */
    static std::vector<std::string> split(const std::string &s, char delimiter) {
        std::vector<std::string> splits;
        std::string split;
        std::istringstream ss(s);

        while (std::getline(ss, split, delimiter)) {
            splits.push_back(split);
        }
        return splits;
    }


    /**
     * @fn std::vector<std::string> splitTokens
     * @brief given a string and a delimiter cut the string into the corresponding values as interpreted in a CSV
     * @param strLine The std::istream containing a repetition of some delimiter
     * @param delimiter The delimiter the csv uses
     * @return A vector of the sliced string
     */
    static std::vector<std::string> splitTokens(std::istream &strLine, const char delimiter) {
        std::string line;
        std::getline(strLine, line);
        return split(line, 44);
    }

};

#endif //CITYOFWEIRDFISHES_STOTALELEMENTS_H

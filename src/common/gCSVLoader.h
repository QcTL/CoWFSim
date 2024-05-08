//
// Created by Laminar on 12/03/2024.
//

#ifndef CITYOFWEIRDFISHES_GCSVLOADER_H
#define CITYOFWEIRDFISHES_GCSVLOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "RelPath.h"

/**
 * @class gCSVLoader
 * @brief Class to load a csv file and return the contents as a vector of vectors of strings.
 */
class gCSVLoader{
public:

    /**
     * @fn std::vector<std::vector<std::string>> givenPath
     * @brief Given a path for the file and the delimitation between cells, returns a matrix with the content of the file
     * @param gCSVPath The name of the file that yo want to extract the values
     * @param cDelimiter A character delimiter of the csv file
     * @return A matrix of strings extracted from the csv file
     */
    static std::vector<std::vector<std::string>> givenPath(const std::string& gCSVPath, const char& cDelimiter){
        std::ifstream fCVS((RelPath::relPath / "files" / "objCollectives" / gCSVPath).string());
        if (!fCVS.is_open())
            std::cerr << "Error opening file." << std::endl;
        std::vector<std::vector<std::string>> gTotal;
        while (!fCVS.eof())
            gTotal.push_back(splitTokens(fCVS, cDelimiter));

        fCVS.close();

        return gTotal;
    }

private:

    /**
     * @fn std::vector<std::string> split
     * @brief given a string and a delimiter cut the string into the corresponding values as interpreted in a CSV
     * @param s The string containing a repetition of some delimiter
     * @param delimiter The delimiter the csv uses
     * @return A vector of the sliced string
     */
    static std::vector<std::string> split(const std::string &s,const char delimiter) {
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
    static std::vector<std::string> splitTokens(std::istream &strLine, const char gSep) {
        std::string line;
        std::getline(strLine, line);
        return split(line, gSep);
    }
};

#endif //CITYOFWEIRDFISHES_GCSVLOADER_H

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

class gCSVLoader{
public:
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

#endif //CITYOFWEIRDFISHES_GCSVLOADER_H

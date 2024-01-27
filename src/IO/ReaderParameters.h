//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_READERPARAMETERS_H
#define CITYOFWEIRDFISHES_READERPARAMETERS_H

#include <map>
#include <fstream>
#include <sstream>

class ReaderParameters {

public:
    static std::map<std::string, std::string>  readFile(const std::string& filePath){
        std::map<std::string, std::string> dAttr;

        std::ifstream file(filePath);
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, ':')) {
                std::string value;
                if (std::getline(is_line, value))
                    dAttr[key] = value;
            }
        }
        return dAttr;
    }
};

#endif //CITYOFWEIRDFISHES_READERPARAMETERS_H

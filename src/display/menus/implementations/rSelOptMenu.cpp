//
// Created by Laminar on 31/01/2024.
//

#include "rSelOptMenu.h"


rSelOptMenu::rSelOptMenu(std::string pthFileD) {
    std::ifstream file(pthFileD);
    nlohmann::json j;
    file >> j;

    std::string type = j["type"];
    std::string src = j["src"];
    std::vector<nlohmann::json> options = j["options"];


    std::ifstream fileTxt(R"(C:\Users\Robert\CityOfWeirdFishes\files\graphic\menus\)" + j["src"]);
    std::string line;
    std::vector<std::vector<int>> data;

    while (std::getline(file, line)) {
        std::vector<int> row;
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        data.push_back(row);
    }
}
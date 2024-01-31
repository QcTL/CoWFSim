//
// Created by Laminar on 31/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RSELOPTMENU_H
#define CITYOFWEIRDFISHES_RSELOPTMENU_H


#include <string>
#include <fstream>
#include "../rIMenu.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <sstream>

class rSelOptMenu : public rIMenu{

public:
    rSelOptMenu(std::string pthFileD);

private:
    std::vector<std::vector<int>> mRawData;
};


#endif //CITYOFWEIRDFISHES_RSELOPTMENU_H

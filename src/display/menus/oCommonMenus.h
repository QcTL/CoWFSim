//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_OCOMMONMENUS_H
#define CITYOFWEIRDFISHES_OCOMMONMENUS_H

#include <string>
#include <vector>

class oCommonMenus{
public:
    static std::string getCompNumber(int nValue){
        std::string numberString = std::to_string(nValue);
        std::vector<std::string> vVal = {"","k","M","kM","B"};
        int nDec=(numberString.length()-1) / 3;

        std::string nLeft;
        if(nDec >= 1){
            nLeft = numberString.substr(0, numberString.length()-nDec*3);
            nLeft = nLeft + "." + numberString[numberString.length()-nDec*3];
        }else{
            nLeft = numberString;
        }

        return nLeft + vVal[nDec];
    }
};

#endif //CITYOFWEIRDFISHES_OCOMMONMENUS_H

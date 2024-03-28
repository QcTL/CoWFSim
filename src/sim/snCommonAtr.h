//
// Created by ganymede on 3/28/24.
//

#ifndef CITYOFWEIRDFISHES_SNCOMMONATR_H
#define CITYOFWEIRDFISHES_SNCOMMONATR_H

#include <string>
#include <map>

class snCommonAtr {
public:
    static int getFlagAtr(const std::string &inStr) { return snCA_mAtr[inStr]; }

    static void setFlagAtr(const std::string &inStr, int inValue) { snCA_mAtr[inStr] = inValue; }

private:
    static std::map<std::string, int> snCA_mAtr;
};

inline std::map<std::string, int>  snCommonAtr::snCA_mAtr;
#endif //CITYOFWEIRDFISHES_SNCOMMONATR_H

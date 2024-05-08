//
// Created by ganymede on 3/28/24.
//

#ifndef CITYOFWEIRDFISHES_SNCOMMONATR_H
#define CITYOFWEIRDFISHES_SNCOMMONATR_H

#include <string>
#include <map>

/**
 * @class snCommonAtr
 * @brief This class is sued to set and access variables that are common for all the classes in the program
 */
class snCommonAtr {
public:
    /**
     * @fn int getFlagAtr
     * @param inStr A valid string used as an identifier that has been inserted before with setFlagAtr
     * @return The value set on that unique string
     */
    static int getFlagAtr(const std::string &inStr) { return snCA_mAtr[inStr]; }

    /**
     * @fn void setFlagAtr
     * @param inStr A valid string
     * @param inValue The value that you want that string to represent
     */
    static void setFlagAtr(const std::string &inStr, int inValue) { snCA_mAtr[inStr] = inValue; }

private:
    static std::map<std::string, int> snCA_mAtr;
};

inline std::map<std::string, int>  snCommonAtr::snCA_mAtr;
#endif //CITYOFWEIRDFISHES_SNCOMMONATR_H

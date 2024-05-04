//
// Created by ganymede on 2/5/24.
//

#ifndef CITYOFWEIRDFISHES_RELPATH_H
#define CITYOFWEIRDFISHES_RELPATH_H

#include <string>
#include <utility>
#include <filesystem>

/**
 * @class RelPath
 * @brief A class that contains the absolute path of the project so it can be used as a start to reference elements relative
 */
class RelPath{
public:
    /**
     * @fn void selRelPath
     * @brief Function to set the absolute total path of the project
     * @param pth A Path to the base of the project
     */
    static void selRelPath(const std::filesystem::path& pth){
        relPath = pth;
    }
    static std::filesystem::path relPath;
};
inline std::filesystem::path RelPath::relPath;
#endif //CITYOFWEIRDFISHES_RELPATH_H

//
// Created by ganymede on 2/5/24.
//

#ifndef CITYOFWEIRDFISHES_RELPATH_H
#define CITYOFWEIRDFISHES_RELPATH_H

#include <string>
#include <utility>
#include <filesystem>

class RelPath{
public:
    static void selRelPath(const std::filesystem::path& pth){
        relPath = pth;
    }
    static std::filesystem::path relPath;
};
inline std::filesystem::path RelPath::relPath;
#endif //CITYOFWEIRDFISHES_RELPATH_H

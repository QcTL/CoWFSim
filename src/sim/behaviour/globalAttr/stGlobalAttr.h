//
// Created by ganymede on 4/6/24.
//

#ifndef CITYOFWEIRDFISHES_STGLOBALATTR_H
#define CITYOFWEIRDFISHES_STGLOBALATTR_H

#include <memory>
#include "../../eyeCatcher/eyeValue.h"

class stGlobalAttr {
public:
    stGlobalAttr() = default;

    static std::shared_ptr<stGlobalAttr> getInstance() {
        if (!instance)
            instance = std::make_shared<stGlobalAttr>();
        return instance;
    }

    eyeValue<uint32_t> stGA_totalPopulation = eyeValue<uint32_t>("Global-Total Population", 0);
    eyeValue<uint32_t> stGA_totalEnergy = eyeValue<uint32_t>("Global-Total Energy", 0);
    eyeValue<uint32_t> stGA_totalImports = eyeValue<uint32_t>("Global-Total Imports", 0);
    eyeValue<uint32_t> stGA_totalExports = eyeValue<uint32_t>("Global-Total Exports", 0);
    eyeValue<double> stGA_ratioEmptyHouses = eyeValue<double>("Global-Ratio Empty House", 0);

private:
    static std::shared_ptr<stGlobalAttr> instance;
};

std::shared_ptr<stGlobalAttr> stGlobalAttr::instance = nullptr;


#endif //CITYOFWEIRDFISHES_STGLOBALATTR_H

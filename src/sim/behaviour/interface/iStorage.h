//
// Created by ganymede on 5/8/24.
//

#ifndef CITYOFWEIRDFISHES_ISTORAGE_H
#define CITYOFWEIRDFISHES_ISTORAGE_H

#include <cstdint>
#include <memory>

template<typename T, typename C>
class iStorage {

    virtual std::shared_ptr<T> getElementByUuid(uint32_t inUuidElem) = 0;

    virtual bool removeElementByUuid(uint32_t inUuidElem) = 0;

    virtual uint32_t addElement(const C& inAddElem) = 0;
};

#endif //CITYOFWEIRDFISHES_ISTORAGE_H

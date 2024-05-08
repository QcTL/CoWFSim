//
// Created by ganymede on 4/8/24.
//

#ifndef CITYOFWEIRDFISHES_IVECTORSTORAGE_H
#define CITYOFWEIRDFISHES_IVECTORSTORAGE_H

#include <cstdint>
#include <utility>
#include <vector>
#include <memory>
#include "interface/iStorage.h"

/**
 * @class iVectorStorage
 * @brief A system of storage with a system of tickets, where in the storing of an element aa index will be given, and its
 * the responsibility of the askee to keep the number as an index, and once its removed, the relation between the index and the inserted
 * object will be lost, because it may be occupied with other elements in the future.
 * @tparam T The type of element you want to store in this class
 */
template<typename T>
class iVectorStorage : public iStorage<T,std::shared_ptr<T>>{
public:
    explicit iVectorStorage(uint32_t inMaxElements) {
        iVS_tVecStorage = std::vector<std::pair<uint32_t, std::shared_ptr<T>>>(inMaxElements, {-1, nullptr});
        for (int i = 0; i < inMaxElements - 1; i++)
            iVS_tVecStorage[i] = {i + 1, nullptr};
        iVS_tVecStorage[iVS_tVecStorage.size() - 1] = {-1, nullptr};
        fEmpty = 0;
    }

    /**
     * @fn std::shared_ptr<T> getElementByUuid
     * @param inUuid A valid index that has been returned with addElement and not yet resolved with removeElementByUuid
     * @return The object stored with that ticket
     */
    std::shared_ptr<T> getElementByUuid(uint32_t inUuidElem) override{
        return iVS_tVecStorage[inUuidElem].second;
    }

    /**
     * @fn uint32_t addElement
     * @param inAddElem A shared pointer to the object you want to save
     * @return A ticket that will reference the object given as parameter if you dont call removeElementByUuid with that ticket
     */
    uint32_t addElement(const std::shared_ptr<T> &inAddElem) override{
        uint32_t prevFEmpty = fEmpty;
        fEmpty = iVS_tVecStorage[fEmpty].first;
        iVS_tVecStorage[prevFEmpty] = {0, inAddElem};
        return prevFEmpty;
    }

    /**
     * @fn void removeElementByUuid
     * @brief removes that element of the storage given the ticket, and from this point, the link between that ticket
     * and the item that was stored to get that ticket will be lost
     * @param inUuidElem A valid ticket of an active element in the storage
     */
    bool removeElementByUuid(uint32_t inUuidElem) override {
        iVS_tVecStorage[inUuidElem] = {fEmpty, {}};
        fEmpty = inUuidElem;
        return true;
    }

protected:
    std::vector<std::pair<uint32_t, std::shared_ptr<T>>> iVS_tVecStorage;
    uint32_t fEmpty;
};

#endif //CITYOFWEIRDFISHES_IVECTORSTORAGE_H

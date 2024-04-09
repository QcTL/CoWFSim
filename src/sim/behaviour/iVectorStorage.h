//
// Created by ganymede on 4/8/24.
//

#ifndef CITYOFWEIRDFISHES_IVECTORSTORAGE_H
#define CITYOFWEIRDFISHES_IVECTORSTORAGE_H

#include <cstdint>
#include <utility>
#include <vector>
#include <memory>

template<typename T>
class iVectorStorage {
public:
    explicit iVectorStorage(uint32_t inMaxElements) {
        iVS_tVecStorage = std::vector<std::pair<uint32_t, std::shared_ptr<T>>>(inMaxElements, {-1, nullptr});
        for (int i = 0; i < inMaxElements - 1; i++)
            iVS_tVecStorage[i] = {i + 1, nullptr};
        iVS_tVecStorage[iVS_tVecStorage.size() - 1] = {-1, nullptr};
        fEmpty = 0;
    }

    std::shared_ptr<T> getElementByUuid(uint32_t inUuid) {
        return iVS_tVecStorage[inUuid].second;
    }

    uint32_t storeElement(const std::shared_ptr<T> &inTElement) {
        uint32_t prevFEmpty = fEmpty;
        fEmpty = iVS_tVecStorage[fEmpty].first;
        iVS_tVecStorage[prevFEmpty] = {0, inTElement};
        return prevFEmpty;
    }

    void removeElement(uint32_t inTPos) {
        iVS_tVecStorage[inTPos] = {fEmpty, {}};
        fEmpty = inTPos;
    }

protected:
    std::vector<std::pair<uint32_t, std::shared_ptr<T>>> iVS_tVecStorage;
    uint32_t fEmpty;
};

#endif //CITYOFWEIRDFISHES_IVECTORSTORAGE_H

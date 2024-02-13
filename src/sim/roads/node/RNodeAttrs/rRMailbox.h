//
// Created by Laminar on 09/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RRMAILBOX_H
#define CITYOFWEIRDFISHES_RRMAILBOX_H

#include <cstdint>
#include <cstdint>
#include <memory>

struct rRMail{
    std::uint32_t mGridOrigin;
    uint32_t mRoadStart;
    uint32_t mSizePath;

    rRMail(uint32_t mGridOrigin, uint32_t mRoadStart, uint32_t mSizePath) : mGridOrigin(mGridOrigin), mRoadStart(mRoadStart),
                                                                          mSizePath(mSizePath) {}
};

#include <queue>
struct rRMailbox{
    std::queue<std::pair<rRMail, uint8_t>> rMB;
};

#endif //CITYOFWEIRDFISHES_RRMAILBOX_H

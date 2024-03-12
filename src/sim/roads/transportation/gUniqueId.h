//
// Created by Laminar on 08/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GUNIQUEID_H
#define CITYOFWEIRDFISHES_GUNIQUEID_H

#include <cstdint>

class gUniqueId{
public:
    static uint32_t gen(uint16_t grid){
        return gMAct[grid]++;
    }

    static uint32_t genGlob() {
        return gGlob++;
    }

private:
    static std::map<uint16_t,uint32_t> gMAct;
    static uint32_t gGlob;
};

std::map<uint16_t,uint32_t> gUniqueId::gMAct;
uint32_t gUniqueId::gGlob;
#endif //CITYOFWEIRDFISHES_GUNIQUEID_H

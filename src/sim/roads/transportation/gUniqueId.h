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
private:
    static std::map<uint16_t,uint32_t> gMAct;
};

std::map<uint16_t,uint32_t> gUniqueId::gMAct;
#endif //CITYOFWEIRDFISHES_GUNIQUEID_H

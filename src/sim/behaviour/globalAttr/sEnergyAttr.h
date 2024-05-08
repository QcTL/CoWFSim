//
// Created by ganymede on 4/10/24.
//

#ifndef CITYOFWEIRDFISHES_SENERGYATTR_H
#define CITYOFWEIRDFISHES_SENERGYATTR_H

#include <cstdint>
#include <vector>

/**
 * @class sEnergyAttr
 * @brief This class is responsible for the calculation of the consumed energy on the simulation
 */
class sEnergyAttr {
public:
    /**
     * @fn uint32_t computeTotalEnergyByState
     * @param inNHouse Quantity of users that are in a home in that moment
     * @param inNOffice Quantity of users that are in the office in that moment
     * @param inRTimer the current reduced time in a simulation < 288
     * @return The total consumption of electricity in that moment
     */
    uint32_t computeTotalEnergyByState(uint32_t inNHouse, uint32_t inNOffice, uint32_t inRTime) {
        return inNHouse * sEA_vecConsHouse[inRTime / 12] + inNOffice * sEA_vecConsOffice[inRTime / 12];
    }
private:
    std::vector<uint32_t> sEA_vecConsHouse = {1, 1, 1, 1, 2, 3, 4, 3, 3, 3, 4, 4, 5, 4, 3, 3, 3, 4, 9, 8, 6, 4, 2, 2};
    std::vector<uint32_t> sEA_vecConsOffice = {1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 2, 2, 3, 3, 2, 2, 2, 3, 2, 1, 1, 1, 1, 1};
};

#endif //CITYOFWEIRDFISHES_SENERGYATTR_H

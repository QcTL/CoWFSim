#include <iostream>
#include <vector>
#include <cstdint>

/**
 * @class rActiveVehicles
 * @brief Class to store the references of cars
 */
class rActiveVehicle {
public:

    static rActiveVehicle initializer;

    rActiveVehicle() {
        initialize(30000);
        fEmpty = 0;
    }

    /**
     * @brief Return the destination of a car given its uuid
     * @param rCar The uuid of the car given when it was added
     * @return A pair with destination index and block index
     */
    static std::pair<uint32_t, uint16_t> getDestByCar(uint32_t rCar){
        return tVecCars[rCar];
    }

    /**
     * @fn uint32_t addCar
     * @brief Store the destinationIndex and DestinationBlock
     * @param destPos The destination index of a given block
     * @param destBlock The id of the destination block
     * @return The new uuid of the car
     */
    static uint32_t addCar(uint32_t destPos, uint16_t destBlock) {
        uint32_t prevFEmpty = fEmpty;
        fEmpty = tVecCars[fEmpty].first;
        tVecCars[prevFEmpty] = {destPos, destBlock};
        return prevFEmpty;
    }

    /**
     * @brief Removes the car from the car storage given the uuid
     * @param rCar The uuid of the car you want to remove, has to exists inside
     */
    static void removeCar(uint32_t rCar) {
        tVecCars[rCar] = {fEmpty, -1};
        fEmpty = rCar;
    }

private:
    static std::vector<std::pair<uint32_t, uint16_t>> tVecCars;
    static uint32_t fEmpty;

    /**
     * @brief initialize the vector to work correctly, it has to be executed when the constructor is called.
     * @param nMaxCars Number of maximum amount of cars that he class will hold
     */
    static void initialize(const uint32_t nMaxCars) {
        tVecCars = std::vector<std::pair<uint32_t, uint16_t>>(nMaxCars, {-1, -1});
        for (int i = 0; i < nMaxCars - 1; i++) {
            tVecCars[i] = {i+1, -1};
            fEmpty = 0;
        }
        tVecCars[tVecCars.size() -1] = {-1, -1};
    }
};

std::vector<std::pair<uint32_t, uint16_t>> rActiveVehicle::tVecCars;
uint32_t rActiveVehicle::fEmpty;
rActiveVehicle rActiveVehicle::initializer;
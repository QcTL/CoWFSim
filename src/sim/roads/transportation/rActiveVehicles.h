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
        initialize(60000);
        fEmpty = 0;
    }

    /**
     * @brief Return the destination of a car given its uuid
     * @param rCar The uuid of the car given when it was added
     * @return A pair with destination index and block index
     */
    static std::pair<uint32_t, uint16_t> getDestByCar(uint32_t rCar) {
        return {tVecCars[rCar].second.rACV_vDestPos, tVecCars[rCar].second.rACV_vDestBlock};
    }

    /**
     * @fn uint32_t addCar
     * @brief Store the destinationIndex and DestinationBlock
     * @param destPos The destination index of a given block
     * @param destBlock The id of the destination block
     * @return The new uuid of the car
     */
    static uint32_t
    addCar(std::pair<uint32_t, uint32_t> inStrPos, std::pair<uint32_t, uint32_t> inEndPos, uint32_t destPos,
           uint16_t destBlock) {
        uint32_t prevFEmpty = fEmpty;
        fEmpty = tVecCars[fEmpty].first;
        tVecCars[prevFEmpty] = {-1, {inStrPos, inEndPos, destPos, destBlock}};
        return prevFEmpty;
    }

    /**
     * @brief Removes the car from the car storage given the uuid
     * @param rCar The uuid of the car you want to remove, has to exists inside
     */
    static void removeCar(uint32_t rCar) {
        tVecCars[rCar] = {(int) fEmpty, {}};
        fEmpty = rCar;
    }

    struct rACV_vehicle {
        std::pair<uint32_t, uint32_t> rACV_vStrPos;
        std::pair<uint32_t, uint32_t> rACV_vEndPos;
        uint32_t rACV_vDestPos;
        uint16_t rACV_vDestBlock;
    };

    static std::pair<uint32_t, uint32_t> getStartPos(uint32_t inUuidCar) {
        return tVecCars[inUuidCar].second.rACV_vStrPos;
    }

    static std::pair<uint32_t, uint32_t> getEndPos(uint32_t inUuidCar) {
        return tVecCars[inUuidCar].second.rACV_vEndPos;
    }

private:
    static std::vector<std::pair<int, rActiveVehicle::rACV_vehicle>> tVecCars;
    static uint32_t fEmpty;

    /**
     * @brief initialize the vector to work correctly, it has to be executed when the constructor is called.
     * @param nMaxCars Number of maximum amount of cars that he class will hold
     */
    static void initialize(const uint32_t nMaxCars) {
        tVecCars = std::vector<std::pair<int, rActiveVehicle::rACV_vehicle>>(nMaxCars, {-1, {}});
        for (int i = 0; i < nMaxCars - 1; i++) {
            tVecCars[i] = {i + 1, {}};
            fEmpty = 0;
        }
        tVecCars[tVecCars.size() - 1] = {-1, {}};
    }
};

std::vector<std::pair<int, rActiveVehicle::rACV_vehicle>> rActiveVehicle::tVecCars;
uint32_t rActiveVehicle::fEmpty;
rActiveVehicle rActiveVehicle::initializer;
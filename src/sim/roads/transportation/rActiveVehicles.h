#include <iostream>
#include <vector>
#include <cstdint>

class rActiveVehicle {
public:

    static rActiveVehicle initializer;
    rActiveVehicle() {
        initialize(15000);
        fEmpty = 0;
    }

    static std::pair<uint32_t, uint16_t> getDestByCar(uint32_t rCar){
        return tVecCars[rCar];
    }

    static uint32_t addCar(uint32_t destPos, uint16_t destBlock) {
        uint32_t prevFEmpty = fEmpty;
        fEmpty = tVecCars[fEmpty].first;
        tVecCars[prevFEmpty] = {destPos, destBlock};
        return prevFEmpty;
    }

    static void removeCar(uint32_t rCar) {
        tVecCars[rCar] = {fEmpty, -1};
        fEmpty = rCar;
    }

private:
    static std::vector<std::pair<uint32_t, uint16_t>> tVecCars;
    static uint32_t fEmpty;

    static void initialize(uint32_t nMaxCars) {
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
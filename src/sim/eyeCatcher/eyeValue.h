//
// Created by ganymede on 4/6/24.
//

#ifndef CITYOFWEIRDFISHES_EYEVALUE_H
#define CITYOFWEIRDFISHES_EYEVALUE_H

#include <iostream>
#include <memory>
#include <cmath>
#include "eyeCatcherActive.h"

template<typename T>
class eyeValue {
public:
    eyeValue() = default;

    eyeValue(const std::string &inValUuid, const T &inStartValue) : eV_uuid(inValUuid), eV_value(inStartValue) {}

    const T &get() const { return eV_value; }

    void set(const T &newValue) {
        eV_value = newValue;
        updateValueEye();
    }

    eyeValue &operator+=(const T &other) {
        eV_value += other;
        updateValueEye();
        return *this;
    }

    eyeValue &operator-=(const T &other) {
        eV_value -= other;
        updateValueEye();
        return *this;
    }

    operator T() const {
        return eV_value;
    }

    void setObserver(const std::shared_ptr<eyeCatcherActive> &inEyeCatcherActive) {
        std::cout << "ADDED OBSERVER" << std::endl;
        eV_isToObserve = true;
        eV_observer = inEyeCatcherActive;
        updateValueEye();
    }

    void removeObserver() {
        std::cout << "REMOVED OBSERVER" << std::endl;
        eV_isToObserve = false;
        eV_observer = nullptr;
    }


private:

    void updateValueEye() {
        if (eV_isToObserve) {
            std::cout << "SEND VALUE" << (int) std::floor(eV_value) << std::endl;
            eV_observer->updateValue((int) std::floor(eV_value), eV_uuid);
        }
    }

    bool eV_isToObserve = false;
    std::string eV_uuid;
    T eV_value;
    std::shared_ptr<eyeCatcherActive> eV_observer;
};

#endif //CITYOFWEIRDFISHES_EYEVALUE_H

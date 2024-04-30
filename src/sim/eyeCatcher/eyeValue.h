//
// Created by ganymede on 4/6/24.
//

#ifndef CITYOFWEIRDFISHES_EYEVALUE_H
#define CITYOFWEIRDFISHES_EYEVALUE_H

#include <iostream>
#include <memory>
#include <cmath>
#include <utility>
#include "eyeCatcherActive.h"


/**
 * @class eyeValue
 * @brief Class that encapsulates a variable that can be tracked and send outside the application
 *
 * @tparam T The type of variable you want to store
 */
template<typename T>
class eyeValue {
public:
    eyeValue() = default;

    /**
     * @brief Eye class constructor
     *
     * @param inValUuid Unique name of the value tracked, structure: NAME_GENERAL-UNIQUE_VALUE
     * @param inStartValue Start value of the variable
     */
    eyeValue(std::string inValUuid, const T &inStartValue) : eV_uuid(std::move(inValUuid)), eV_value(inStartValue) {}


    /**
     * @fn T get()
     * @brief Function to get the current value of the variable
     *
     * @return The actual value of the variable type T
     */
    const T &get() const { return eV_value; }

    /**
     * @brief Set the value of the eyeValue.
     *
     * This function sets the value of the eyeValue object to the provided newValue.
     * If the new value is equal to the current value, the function returns without performing any further actions.
     * After setting the new value, the function calls updateValueEye().
     *
     * @param newValue The new value to set.
     */
    void set(const T &newValue) {
        if (eV_value == newValue)
            return;
        eV_value = newValue;
        updateValueEye();
    }

    /**
     * @brief Add a value to the variable of eyeValue.
     *
     * This operator adds the provided value to the current value of the eyeValue object.
     *
     * @param other The value to add.
     * @return A reference to the modified eyeValue object.
     */
    eyeValue &operator+=(const T &other) {
        eV_value += other;
        updateValueEye();
        return *this;
    }

    /**
     * @brief Subtract a value from the eyeValue.
     *
     * This operator subtracts the provided value from the current value of the eyeValue object.
     * @param other The value to subtract.
     * @return A reference to the modified eyeValue object.
     */
    eyeValue &operator-=(const T &other) {
        eV_value -= other;
        updateValueEye();
        return *this;
    }

    /**
     * @brief Conversion operator to type T.
     *
     * This conversion operator allows implicit conversion of the eyeValue object to type T.
     *
     * @return The value of the eyeValue object.
     */
    operator T() const {
        return eV_value;
    }

    /**
     * @brief Set the observer for the eyeValue.
     *
     * This function sets an observer for the eyeValue object.
     * It sets eV_isToObserve to true, assigns the provided observer to eV_observer
     *
     * @param inEyeCatcherActive The observer to set.
     */
    void setObserver(const std::shared_ptr<eyeCatcherActive> &inEyeCatcherActive) {
        eV_observer = inEyeCatcherActive;
        updateValueEye();
    }

    /**
     * @brief Remove the observer from the eyeValue.
     *
     * This function removes the observer from the eyeValue object.
     */
    void removeObserver() {
        eV_observer = nullptr;
    }

    /**
     * @brief Check if the eyeValue is being observed.
     *
     * This function checks if the eyeValue object has an observer assigned to it. And is sending information
     * of the new value outside the application
     *
     * @return true if the eyeValue is being observed, false otherwise.
     */
    [[nodiscard]] bool isObserved() const {
        return eV_observer != nullptr;
    }


private:

    /**
     * @brief Sends the updated to the active observer if it has associated one.
     *
     * If the function have an observer attached will call its function to send the new value outside the application.
     *
     */
    void updateValueEye() {
        if (isObserved())
            eV_observer->updateValue((int) std::floor(eV_value), eV_uuid);
    }

    std::string eV_uuid;
    T eV_value;
    std::shared_ptr<eyeCatcherActive> eV_observer;
};

#endif //CITYOFWEIRDFISHES_EYEVALUE_H

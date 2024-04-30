//
// Created by ganymede on 4/6/24.
//

#ifndef CITYOFWEIRDFISHES_EYECATCHERACTIVE_H
#define CITYOFWEIRDFISHES_EYECATCHERACTIVE_H

#include <vector>
#include <string>
#include "eyeCatcherConnection.h"
#include "./eyeTotalTick.h"

/**
 * @class eyeCatcherActive
 * @brief Unique Observer that all the eyeValues share if want to be extracted when changed.
 */
class eyeCatcherActive {
public:

    /**
     * @fn void updateValue
     * @brief Observer function called when the value of the eye is changed
     * @param inNewValue The new value of the @ref eyeValue observed
     * @param inUuidEye The string uuid of the @ref EyeValue observed
     */
    void updateValue(int inNewValue, const std::string &inUuidEye) {
        if (eCA_eyeCatcherConnection)
            eCA_eyeCatcherConnection->sendInformation(inNewValue, inUuidEye, eyeTotalTick::getTick());
        else
            std::cerr << "Error: eyeCatcherConnection is nullptr." << std::endl;

    }

    /**
     * @fn std::shared_ptr<eyeCatcherActive> getInstance
     * @brief Static function to get the shared reference observer where they share a single @ref eyeCatcherConnection
     * @param inEyeCC A shared pointer of the @ref eyeCatcherConnection that this class will have
     * @return A reference to itself.
     */
    static std::shared_ptr<eyeCatcherActive> getInstance(const std::shared_ptr<eyeCatcherConnection> &inEyeCC) {
        if (!instance) {
            instance = std::make_shared<eyeCatcherActive>(inEyeCC);
        }
        return instance;
    }

    /**
     * @fn td::shared_ptr<eyeCatcherActive> getInstance
     * @brief Static function to get the shared reference observer where they share a single @ref eyeCatcherConnection
     * @return A reference to itself.
     */
    static std::shared_ptr<eyeCatcherActive> getInstance() {
        return instance;
    }

    /**
     * @brief basic constructor what only @ref std::shared_ptr<eyeCatcherActive> getInstance should call.
     * @param inEyeCC Pointer to @ref eyeCatcherConnection that implements the way in how to send the value of @ref eyeValue when changed.
     */
    explicit eyeCatcherActive(const std::shared_ptr<eyeCatcherConnection> &inEyeCC)
            : eCA_eyeCatcherConnection(inEyeCC) {}

private:
    std::shared_ptr<eyeCatcherConnection> eCA_eyeCatcherConnection;
    static std::shared_ptr<eyeCatcherActive> instance;
};

#endif //CITYOFWEIRDFISHES_EYECATCHERACTIVE_H

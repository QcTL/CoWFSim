//
// Created by ganymede on 4/6/24.
//

#ifndef CITYOFWEIRDFISHES_EYECATCHERACTIVE_H
#define CITYOFWEIRDFISHES_EYECATCHERACTIVE_H

#include <vector>
#include <string>
#include "eyeCatcherConnection.h"

class eyeCatcherActive {
public:
    void updateValue(int inNewValue, const std::string &inUuidEye) {
        std::cout << "SEND INFORMATION" << inNewValue << std::endl;
        if (eCA_eyeCatcherConnection) {
            eCA_eyeCatcherConnection->sendInformation(inNewValue, inUuidEye);
        } else {
            std::cerr << "Error: eyeCatcherConnection is nullptr." << std::endl;
        }
    }

    static std::shared_ptr<eyeCatcherActive> getInstance(const std::shared_ptr<eyeCatcherConnection> &inEyeCC) {
        if (!instance) {
            instance = std::make_shared<eyeCatcherActive>(inEyeCC);
        }
        return instance;
    }

    static std::shared_ptr<eyeCatcherActive> getInstance() {
        return instance;
    }

    explicit eyeCatcherActive(const std::shared_ptr<eyeCatcherConnection> &inEyeCC)
            : eCA_eyeCatcherConnection(inEyeCC) {}

private:
    std::shared_ptr<eyeCatcherConnection> eCA_eyeCatcherConnection;
    static std::shared_ptr<eyeCatcherActive> instance;
};

#endif //CITYOFWEIRDFISHES_EYECATCHERACTIVE_H

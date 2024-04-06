//
// Created by ganymede on 4/6/24.
//

#ifndef CITYOFWEIRDFISHES_EYECATCHERCONNECTION_H
#define CITYOFWEIRDFISHES_EYECATCHERCONNECTION_H

#include <memory>
#include <chrono>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

class eyeCatcherConnection {
public:
    explicit eyeCatcherConnection() {
        eyC_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (eyC_serverSocket < 0) {
            std::cerr << "Error opening socket" << std::endl;
            exit(1);
        }
        // Bind the socket to the IP and port
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(8888);
        if (bind(eyC_serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
            std::cerr << "Error on binding" << std::endl;
            exit(1);
        }

        // Listen for incoming connections
        listen(eyC_serverSocket, 5);

        // Accept a client connection
        socklen_t clientAddressLength;
        sockaddr_in clientAddress;
        clientAddressLength = sizeof(clientAddress);
        eyC_clientAddress = accept(eyC_serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
        if (eyC_clientAddress < 0) {
            std::cerr << "Error on accept" << std::endl;
            exit(1);
        }
    };

    struct eyC_DataPacket {
        int eyc_nValue;
        char eyC_uuidEyeValue[32];
    };

    void sendInformation(int inNewValue, const std::string &inUuidEye) const {
        eyC_DataPacket inEyC_DP = {inNewValue, ""};
        strncpy(inEyC_DP.eyC_uuidEyeValue, inUuidEye.c_str(), sizeof(inEyC_DP.eyC_uuidEyeValue) - 1);
        inEyC_DP.eyC_uuidEyeValue[sizeof(inEyC_DP.eyC_uuidEyeValue) - 1] = '\0';
        for (size_t i = inUuidEye.length(); i < sizeof(inEyC_DP.eyC_uuidEyeValue); ++i) {
            inEyC_DP.eyC_uuidEyeValue[i] = ' ';
        }
        send(eyC_clientAddress, &inEyC_DP, sizeof(inEyC_DP), 0);
    }

    ~eyeCatcherConnection() {
        close(eyC_clientAddress);
        close(eyC_serverSocket);
    }

private:
    int eyC_serverSocket;
    int eyC_clientAddress;
};

#endif //CITYOFWEIRDFISHES_EYECATCHERCONNECTION_H

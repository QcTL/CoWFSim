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

/**
 * @class eyeCatcherConnection
 * @brief class to send the eyeValues changed outside of the application, in this case using sockets.
 * Class to connect to the local website where the active eyeValues are displayed, using port 8888 and localhost
 */
class eyeCatcherConnection {
public:
    /**
     * @brief constructor of class that also open the necessary sockets to send information
     * Opens the socked 8888 as a server side to send information, also accepts the client side of the connection.
     */
    explicit eyeCatcherConnection() {
        std::cout << "WAITING FOR WEB CONNECTION" << std::endl;

        eyC_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (eyC_serverSocket < 0) {
            std::cerr << "Error opening socket" << std::endl;
            exit(1);
        }

        int yes = 1;
        if (setsockopt(eyC_serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            std::cerr << "Error setting socket option" << std::endl;
            exit(1);
        }

        // Bind the socket to the IP and port
        sockaddr_in serverAddress{};
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
        sockaddr_in clientAddress{};
        clientAddressLength = sizeof(clientAddress);
        eyC_clientAddress = accept(eyC_serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
        if (eyC_clientAddress < 0) {
            std::cerr << "Error on accept" << std::endl;
            exit(1);
        }
    };

    /**
     * @struct eyC_DataPacket
     * @brief collection of information that will be send in a package from server to client.
     * It consist of 3 elements, the new number, the timestamp where it changed and the eyC uuid in a char[32] padding spaces left.
     */
    struct eyC_DataPacket {
        int eyC_nValue;
        int eyeC_nTime;
        char eyC_uuidEyeValue[32];
    };


    /**
     * @fn void sendInformation
     * @brief Send the package @ref eyC_DataPacket to the connected client extracted from the parameters
     * @param inNewValue The new value the @ref eyeValue got
     * @param inUuidEye The uuid name of the @ref eyeValue changed
     * @param inTotalTick The timestamp where this change happened.
     */
    void sendInformation(int inNewValue, const std::string &inUuidEye, int inTotalTick) const {
        eyC_DataPacket inEyC_DP = {inNewValue, inTotalTick, ""};
        strncpy(inEyC_DP.eyC_uuidEyeValue, inUuidEye.c_str(), sizeof(inEyC_DP.eyC_uuidEyeValue) - 1);
        inEyC_DP.eyC_uuidEyeValue[sizeof(inEyC_DP.eyC_uuidEyeValue) - 1] = '\0';
        for (size_t i = inUuidEye.length(); i < sizeof(inEyC_DP.eyC_uuidEyeValue); ++i) {
            inEyC_DP.eyC_uuidEyeValue[i] = ' ';
        }
        send(eyC_clientAddress, &inEyC_DP, sizeof(inEyC_DP), 0);
    }

    /**
     * @brief destructor where the connection is closed.
     */
    ~eyeCatcherConnection() {
        close(eyC_clientAddress);
        close(eyC_serverSocket);
    }

private:
    int eyC_serverSocket;
    int eyC_clientAddress;
};

#endif //CITYOFWEIRDFISHES_EYECATCHERCONNECTION_H

//
// Created by pc on 2/28/25.
//

// SERVER -> VISION PC
// CLIENT -> CONTROL PC

#include "server.hpp"

int main()
{
    int serverSocket;
    int clientSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == 0)
    {
        perror("Socket Failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind Failed");
        return -1;
    }

    if (listen(serverSocket, 8) < 0)
    {
        perror("Listen Failed");
        return -1;
    }
    std::cout << "Server Listening on port " << PORT << "..." << std::endl;

    clientSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (clientSocket < 0)
    {
        perror("Accept Failed");
        return -1;
    }

    // 수정
    while (true)
    {
        receiveData(clientSocket, robot.torque);
        receiveData(clientSocket, robot.motorpos);

        std::cout << "torque : " << robot.torque << std::endl;
        std::cout << "motorpos : " << robot.motorpos << std::endl;

        vision.footstep[0] = 0.1;
        vision.footstep[1] = 1.2;
        vision.footstep[2] = 2.3;
        vision.footstep[3] = 3.4;

        for (int idx = 0; idx < 4; idx++)
        {
            sendData(clientSocket, vision.footstep[idx]);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // 수정

    close(clientSocket);
    close(serverSocket);

    return 0;
}

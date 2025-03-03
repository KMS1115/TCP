//
// Created by pc on 2/28/25.
//

// SERVER -> VISION PC
// CLIENT -> CONTROL PC

#include "client.hpp"

int main()
{
    int clientSocket;
    struct sockaddr_in address;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        perror("Socket Failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &address.sin_addr) <= 0)
    {
        perror("Invalid address / Address not supported");
        return -1;
    }

    if (connect(clientSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // 수정
    while (true)
    {
        robot.torque += 0.1;
        robot.motorpos += 0.2;

        sendData(clientSocket, robot.torque);
        sendData(clientSocket, robot.motorpos);

        for (int idx = 0; idx < 4; idx++)
        {
            receiveData(clientSocket, vision.footstep[idx]);
            std::cout << "vision[" << idx << "] : " << vision.footstep[idx] << std::endl;
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // 수정
}

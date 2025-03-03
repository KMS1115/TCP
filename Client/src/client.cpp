//
// Created by pc on 2/28/25.
//

// SERVER -> VISION PC
// CLIENT -> CONTROL PC

#include "client.hpp"

#define PORT 8000
#define SERVER_IP "10.125.63.97"

struct RobotState
{
    double torque;
    double motorpos;
};

struct Vision
{
    double footstep[4];
};

RobotState robot;
Vision vision;
uint64_t temp;

uint64_t htonll(uint64_t value)
{
    uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
    uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFF));
    return (static_cast<uint64_t>(low_part) << 32) | high_part;
}

uint64_t ntohll(uint64_t value)
{
    uint32_t high_part = ntohl(static_cast<uint32_t>(value >> 32));
    uint32_t low_part = ntohl(static_cast<uint32_t>(value & 0xFFFFFFFF));
    return (static_cast<uint64_t>(low_part) << 32) | high_part;
}

int main()
{
    int clientSocket;
    struct sockaddr_in address;

    // 1. 클라이언트 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        perror("Socket Failed");
        return -1;
    }

    // 2. 서버 주소 설정 (localhost:8000)
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &address.sin_addr) <= 0)
    {
        perror("Invalid address / Address not supported");
        return -1;
    }

    // 3. 서버에 연결 요청
    if (connect(clientSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // 4. 데이터 송신 및 수신

    while (true)
    {
        robot = {123.456, 789.012};
        memcpy(&temp, &robot.torque, sizeof(temp));
        temp = htonll(temp);
        send(clientSocket, &temp, sizeof(temp), 0);

        memcpy(&temp, &robot.motorpos, sizeof(temp));
        temp = htonll(temp);
        send(clientSocket, &temp, sizeof(temp), 0);

        recv(clientSocket, &vision, sizeof(vision), 0);

        for (int idx = 0; idx < 4; idx++)
        {
            memcpy(&temp, &vision.footstep[idx], sizeof(temp));
            temp = ntohll(temp);
            memcpy(&vision.footstep[idx], &temp, sizeof(temp));
            std::cout << "vision[" << idx << "] : " << vision.footstep[idx] << std::endl;
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 5. 소켓 종료
    close(clientSocket);

    return 0;
}

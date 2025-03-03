//
// Created by pc on 2/28/25.
//

// SERVER -> VISION PC
// CLIENT -> CONTROL PC

#include "server.hpp"

#define PORT 8000

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
    int serverSocket;                        // 서버와 클라이언트의 연결을 기다리는 소켓
    int clientSocket;                        // 클라이언트와 직접적으로 데이터를 주고 받는 소켓
    struct sockaddr_in address;              // 소켓 주소 구조체 -> 소켓 생성 및 바인딩할 때 사용
    int addrlen = sizeof(address);           // 주소의 크기를 저장할 때 사용(구조체의 크기 저장)

    // 1. 서버 소켓 생성
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == 0)
    {
        perror("Socket Failed");
        return -1;
    }

    // 2. 주소 설정
    address.sin_family = AF_INET;            // AF_INET : IPv4를 사용해서 통신
    address.sin_addr.s_addr = INADDR_ANY;    // INADDR_ANY : 어떤 IP 주소에서도 연결 가능
    address.sin_port = htons(PORT);          // 서버가 수신할 포트 번호를 설정(8000)

    // 3. 바인딩
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind Failed");
        return -1;
    }

    // 4. 클라이언트 연결 대기
    if (listen(serverSocket, 8) < 0)
    {
        perror("Listen Failed");
        return -1;
    }
    std::cout << "Server Listening on port " << PORT << "..." << std::endl;

    // 5. 클라이언트 연결 수락
    clientSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (clientSocket < 0)
    {
        perror("Accept Failed");
        return -1;
    }

    // 6. 데이터 송신 및 수신
    while (true)
    {
        int bytesReceived = recv(clientSocket, &temp, sizeof(temp), 0);
        if (bytesReceived <= 0) break;

        temp = ntohll(temp);
        memcpy(&robot.torque, &temp, sizeof(robot.torque));

        recv(clientSocket, &temp, sizeof(temp), 0);
        temp = ntohll(temp);
        memcpy(&robot.motorpos, &temp, sizeof(robot.motorpos));

        std::cout << "torque : " << robot.torque << std::endl;
        std::cout << "motorpos : " << robot.motorpos << std::endl;

        vision.footstep[0] = 0.1;
        vision.footstep[1] = 1.2;
        vision.footstep[2] = 2.3;
        vision.footstep[3] = 3.4;

        for (int idx = 0; idx < 4; idx++)
        {
            memcpy(&temp, &vision.footstep[idx], sizeof(temp));
            temp = htonll(temp);
            memcpy(&vision.footstep[idx], &temp, sizeof(temp));
        }

        send(clientSocket, &vision, sizeof(vision), 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 7. 소켓 종료
    close(clientSocket);
    close(serverSocket);

    return 0;
}

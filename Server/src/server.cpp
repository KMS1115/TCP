//
// Created by pc on 2/28/25.
//

#include "server.hpp"

#define PORT 8000

struct RobotState
{
    int torque;
    int motorpos;
};

struct Vision
{
    int footstep[4];
};

RobotState robot;
Vision vision;

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
    if (clientSocket < 0) {
        perror("Accept Failed");
        return -1;
    }

    // 6. 클라이언트에서 데이터 받기
    read(clientSocket, &robot, sizeof(robot));

    robot.torque = ntohl(robot.torque);
    robot.motorpos = ntohl(robot.motorpos);

    // 7. 클라이언트로 데이터 보내기
    for (int idx = 0; idx < 4; idx++)
    {
        vision.footstep[idx] = ntohl(vision.footstep[idx]);
    }

    send(clientSocket, &vision, sizeof(vision), 0);

    // 8. 소켓 종료
    close(clientSocket);
    close(serverSocket);

    return 0;
}

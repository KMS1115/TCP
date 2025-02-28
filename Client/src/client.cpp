//
// Created by pc on 2/28/25.
//

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8000

// 로봇 상태 데이터 구조체
struct RobotState {
    float battery;
    float velocity;
    float position[3]; // x, y, z
};

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // 1. 클라이언트 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 2. 서버 주소 설정 (127.0.0.1:8080)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return -1;
    }

    // 3. 서버에 연결 요청
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // 4. 풋스텝 데이터 전송
    float footstep[6] = {0.5, 1.2, 0.3, 0.0, 0.1, 0.2}; // (x, y, z, roll, pitch, yaw)
    send(sock, footstep, sizeof(footstep), 0);
    std::cout << "Sent footstep: ["
              << footstep[0] << ", " << footstep[1] << ", " << footstep[2] << ", "
              << footstep[3] << ", " << footstep[4] << ", " << footstep[5] << "]\n";

    // 5. 서버로부터 로봇 상태 데이터 수신
    RobotState state;
    read(sock, &state, sizeof(state));
    std::cout << "Received robot state: [Battery: " << state.battery << ", Velocity: "
              << state.velocity << ", Position: (" << state.position[0] << ", "
              << state.position[1] << ", " << state.position[2] << ")]\n";

    // 6. 소켓 종료
    close(sock);
    return 0;
}

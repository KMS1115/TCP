//
// Created by pc on 2/28/25.
//

#include "server.hpp"

#define PORT 8080

struct RobotState;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int number;

    // 1. 서버 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return -1;
    }

    // 2. 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 3. 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }

    // 4. 클라이언트 연결 대기
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return -1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // 5. 클라이언트 연결 수락
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        return -1;
    }

    // 6. 클라이언트에서 정수 받기
    read(new_socket, &number, sizeof(number));
    number = ntohl(number);  // 네트워크 바이트 오더 변환
    std::cout << "Received from client: " << number << std::endl;

    // 7. 숫자를 2배로 만들어서 다시 클라이언트로 전송
    int response = htonl(number * 2);
    send(new_socket, &response, sizeof(response), 0);
    std::cout << "Sent back: " << number * 2 << std::endl;

    // 8. 소켓 종료
    close(new_socket);
    close(server_fd);

    return 0;
}

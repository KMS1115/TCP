//
// Created by pc on 3/3/25.
//

#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>
#include <thread>
#include <chrono>

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

template <typename T>
void sendData(int socket, const T& data) {
    memcpy(&temp, &data, sizeof(temp));
    temp = htonll(temp);
    send(socket, &temp, sizeof(temp), 0);
}

template <typename T>
void receiveData(int socket, T& data) {
    recv(socket, &temp, sizeof(temp), 0);
    temp = ntohll(temp);
    memcpy(&data, &temp, sizeof(data));
}


#endif //COMMON_HPP

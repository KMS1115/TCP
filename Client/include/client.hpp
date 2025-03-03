//
// Created by pc on 2/28/25.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "common.hpp"

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

#endif //CLIENT_HPP

//
// Created by pc on 2/28/25.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.hpp"

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

#endif //SERVER_HPP

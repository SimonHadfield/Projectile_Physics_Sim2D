#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <vector>
#include <chrono>

float gravity(float v_velocity);
std::vector<float> updatePos(float v_velocity, float h_velocity, float del_time, bool grav_on);

#endif
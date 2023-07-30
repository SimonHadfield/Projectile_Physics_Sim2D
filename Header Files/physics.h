#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <chrono>

float gravity(float v_velocity, float timestep);
std::vector<float> updatePos(std::vector<float> position, float v_velocity, float h_velocity, float timestep);

#endif
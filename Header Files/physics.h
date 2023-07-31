#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <chrono>

void gravity(float v_velocity);
void updatePos(std::vector<float> position, float v_velocity, float h_velocity);

#endif
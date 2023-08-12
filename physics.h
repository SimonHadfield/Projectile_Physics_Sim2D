#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"

float gravity(float v_velocity);
glm::vec2 updatePos(glm::vec2 velocity, float del_time, bool grav_on);

#endif
#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"

float gravity(float v_velocity);
glm::vec2 updatePos(glm::vec2 velocity, float del_time, bool grav_on);
struct AABB{glm::vec2 min;glm::vec2 max;};
bool AABBIntersect(const AABB& a, const AABB& b);
#endif
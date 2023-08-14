#include "physics.h"

//physics functions
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//gravity state
float gravity(bool grav_on) //vertical velocity
{
	float g = -0.00025f;
	return grav_on ? g : 0;
}

//change in position
glm::vec2 updatePos(glm::vec2 velocity, float del_Time, bool grav_on)
{
	//2D vector x and y
	// del_x += v_v*del_Time, del_y = h_v+del_Time
	float g = gravity(grav_on);
	float del_v = g * del_Time;
	velocity.y += del_v;
	glm::vec2 del_position(velocity.x * del_Time, velocity.y * del_Time);
	
	return del_position;
}

bool AABBIntersect(const AABB& a, const AABB& b) {
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
	return true;
}

glm::mat4 CalculateCollisionResolutionMatrix(const AABB& a, const AABB& b)
{
	glm::vec2 collisionNormal = glm::normalize(a.min - a.max);
	glm::vec2 mtv = (a.max - a.min) * collisionNormal; //
	glm::vec2 separationVector = mtv * 0.5f; // half mtv for each object
	glm::mat4 resolutionMatrix = glm::translate(glm::mat4(0.0f), glm::vec3(separationVector, 0.0f));
	return resolutionMatrix;
}
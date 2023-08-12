#include "physics.h"

//physics functions
#include "glm/glm.hpp"

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
// a*del_Time = del_v
// del_x = v * del_Time + additional vel
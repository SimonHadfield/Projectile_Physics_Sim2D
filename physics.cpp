#include "physics.h"

//physics functions


//gravity state
float gravity(bool grav_on) //vertical velocity
{
	float g = -0.00025f;
	return grav_on ? g : 0;
}

//change in position
std::vector<float> updatePos(float v_velocity, float h_velocity, float del_time, bool grav_on)
{
	//2D vector x and y
	// del_x += v_v*del_time, del_y = h_v+del_time
	float g = gravity(grav_on);
	float del_v = g * del_time;
	std::vector<float> del_position = {h_velocity * del_time, (v_velocity + del_v) * del_time , v_velocity + del_v };
	
	return del_position;
}
// a*del_time = del_v
// del_x = v * del_time + additional vel
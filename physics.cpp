#include "physics.h"

//physics functions

std::vector<float> position = { 0.0, 0.0 };

//gravity - return downward velocity as float

void gravity(float v_velocity) //vertical velocity
{
	// given an initial velocity and timestep, compute final velocity from 10m/s^2 acceleration
	float g = 10.0;
	// final_v = initial_v + g * t
	v_velocity -= g;
}

void updatePos(std::vector<float> anchor_position, float v_velocity, float h_velocity)
{
	//2D vector x and y
	// x += v_v*t, y + h_v+t
	//gravity(v_velocity);
	v_velocity = 0.01f;
	anchor_position[0] += h_velocity * 1.0f;
	anchor_position[1] += v_velocity * 1.0f;
}
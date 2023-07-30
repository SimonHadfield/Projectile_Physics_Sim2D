#include "physics.h"

//physics functions

std::vector<float> position = { 0.0, 0.0 };

//gravity - return downward velocity as float

float gravity(float v_velocity, float timestep) //vertical velocity
{
	// given an initial velocity and timestep, compute final velocity from 10m/s^2 acceleration
	float g = 10.0;
	// final_v = initial_v + g * t
	return v_velocity + g * timestep;
}

std::vector<float> updatePos(std::vector<float> position, float v_velocity, float h_velocity, float timestep)
{
	//2D vector x and y
	// x += v_v*t, y + h_v+t
	position[0] += v_velocity * timestep;
	position[1] += h_velocity * timestep;
	return position;
}
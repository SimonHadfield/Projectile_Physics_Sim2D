#include "physics.h"

//physics functions


//gravity - return downward velocity as float

float gravity(float v_velocity, float timestep)
{
	// given an initial velocity and timestep, compute final velocity from 10m/s^2 acceleration
	float g = 10.0;
	// final_v = initial_v + g * t
	return v_velocity + g * timestep;
}
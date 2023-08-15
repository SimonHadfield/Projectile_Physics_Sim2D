#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;
uniform mat4 transformationMatrix; //transformation for animation
uniform mat4 collisionresolutionMatrix; // transformation for resolving collisions

void main()
{
	vec4 collision_resolution = collisionresolutionMatrix * position; // collisionresolutionMatrix* position;
	//vec4 translated_position = (transformationMatrix * collision_resolution);
	vec4 translated_position = (transformationMatrix * position);
	gl_Position = u_MVP * translated_position; //apply transformationMatrix then MVP 
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
   color = u_Color;
};
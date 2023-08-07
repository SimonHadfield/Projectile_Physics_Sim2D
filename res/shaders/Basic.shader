#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

//uniform vec2 u_position;

void main()
{
	//gl_Position = u_position;
	gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
   color = u_Color;
};
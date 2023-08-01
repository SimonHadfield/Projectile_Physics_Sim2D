# Physics_Sim2D
C++ physics simulation using opengl

CURRENT Task:

- main.cpp / window.cpp / window.h
	- set up the glfw window context and display screen using two separate files
	- set up renderer and abstract window functions into classes


TODO:

- main.cpp:
	- intialize window separate to rendering window: READ OTHER PEOPLE CODE AND LAYOUT (window.cpp getting messy - where do i call gravity and updatepos?)

- shader.cpp and shader.h:
	- creatation of shaders for rendering
	- vertex and fragment shader code to handle rendering projectile's trajectory?

- projectile.cpp and projectile.h
	- these files will define the projectile class
	- class will store attributes like position, velocity, acceleration and other data for simulation

- physics.cpp and physics.h:
	- these files will define the physics class e.g newton's laws and drag

- renderer.cpp and renderer.h:
	- rendering projectile's trajectory and other graphics
	- handle vertex buffer objects (VBOs), vertex array objects (VAOs) and other opengl related tasks

- window.cpp and window.h:
	- these files will define the window class using opengl window and handling user inputs (closing window)

- mathutils.cpp and mathutils.h
	- contain utility functions for mathematical calculations, such as vector operations and transformations
	- (vector calculations for physics or opengl?)

- resources/ :
	- shader files, texture files (how does this vary from shader.cpp, does .cpp load these files in?)

- cmakelists.txt:
	- define the projects build settings, dependencies and compilation instructions
	- for cmake it will configure project to include libraries (e.g opengl, glfw)


#include "window.h"
#include "physics.h"

int CreateWindow(bool displayState, float offset) {
    std::cout << displayState << std::endl;
    if (!displayState)
        return -2; //end display

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit()!= GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    //initialize position and velocity of object
    std::vector<float> anchor_position = { 0.0f,0.0f }; // initialize triangle position x y
    float v_velocity = 0.01f;
    float h_velocity = 0.01f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        float positions[6] = {
            -0.5f + anchor_position[0], -0.5f + anchor_position[1],
             0.0f + anchor_position[0],  0.5f + anchor_position[1],
             0.5f + anchor_position[0], -0.5f + anchor_position[1]
        };

        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
        // Animation update
        
        std::cout << v_velocity << std::endl;
        updatePos(anchor_position, v_velocity, h_velocity);
 
        glfwWaitEventsTimeout(1.0); //approx 60fps
        std::cout << "Y: " << anchor_position[1] << std::endl;
        std::cout << "X: " << anchor_position[0] << std::endl;

        //glfwWaitEventsTimeout(0.016); //approx 60fps
    }

    glfwTerminate();

}
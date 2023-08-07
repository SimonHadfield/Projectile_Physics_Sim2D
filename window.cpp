#include "window.h"
#include "physics.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

int CreateWindow(bool displayState) {
    std::cout << displayState << std::endl;
    if (!displayState)
        return -2; //end display

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit()!= GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    //velocity of object
    float v_velocity = 0.00f;
    float h_velocity = 0.00f;

    bool object_stationary = true;

    if (object_stationary)
    {
        v_velocity = 0.00f;
        h_velocity = 0.00f;
    }
    else if (!object_stationary)
    {
        v_velocity = 0.01f;
        h_velocity = 0.01f;
    }

    {
        float positions[] = {
                -0.05f - 0.5f, -0.05f, //index 1
                 0.05f - 0.5f, -0.05f, //index 2
                 0.05f - 0.5f,  0.05f, //index 3
                -0.05f - 0.5f,  0.05f  //index 4
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        //create vertex array (vao)
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        //create buffer
        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb,layout);

        // create index buffer
        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        //color uniform
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        //unbind everything
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        float r = 0.0f;
        float increment = 0.005f;
        bool grav_on = false;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            // userinput
            std::cout << "Grav state : " << grav_on << std::endl;
            if ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) && !grav_on)
                grav_on = true;
            //else if ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) && grav_on)
            //    grav_on = false;
            if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS))
            {
                v_velocity = 0.015f; h_velocity = 0.008f; grav_on = true;
            }
            if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS))
            {
                v_velocity = 0.00f; h_velocity = 0.00f; grav_on = false;
                
            }

            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));


            // Animation:
            float time = glfwGetTime(); // get change in time
            float del_time = 0.50f;

            std::vector<float> del_position = updatePos(v_velocity, h_velocity, del_time, grav_on);
            v_velocity = del_position[2];

            for (int i = 0; i < (sizeof(positions) / sizeof(float)); i++)
            {
                if (!(i % 2))
                    positions[i] += del_position[0]; //move x coordinate
                else if (i % 2)
                    positions[i] += del_position[1]; //move y coordinate
            }

            //create buffer
            VertexArray va;
            VertexBuffer vb(positions, 4 * 2 * sizeof(float));
            VertexBufferLayout layout;
            layout.Push<float>(2);
            va.AddBuffer(vb, layout);

            //bind
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.005f;
            else if (r < 0.0f)
                increment = 0.005f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            //glfwWaitEventsTimeout(0.016); //approx 60fps
        }
    }
    glfwTerminate();

}
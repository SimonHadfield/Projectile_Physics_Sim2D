#include "window.h"
#include "physics.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

#include "stb_image/stb_image.h"

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
        //square projectile
        float positions[] = {
                -0.55f, -0.05f, //index 0 (x,y)
                -0.48f, -0.05f, //index 1
                -0.48f,  0.05f, //index 2
                -0.55f,  0.05f  //index 3
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        //platform pos - same top height as bottom of projectile, bottom height -1.0f
        float pl_positions[] = {
                -1.00f,  -1.00f, //index 0
                -0.40f,  -1.00f, //index 1
                -0.40f,  -0.05f, //index 2
                -1.00f,  -0.05f  //index 3
        };

        //floor pos
        float fl_positions[] = {
                -1.00f,  -1.00f, //index 0
                 1.40f,  -1.00f, //index 1
                 1.40f,  -0.70f, //index 2
                -1.00f,  -0.70f  //index 3
        };
        unsigned int fl_indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        //create vertex array (vao)
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        ///// projectile
        // 
        //create buffer - projectile
        VertexArray va; // square vertex array
        VertexBuffer vb(positions, 4 * 2 * sizeof(float)); //vertex buffer

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb,layout);

        // create index buffer - projectile
        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        //color uniform
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f); //rgba

        //unbind everything
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        ///// platform
        // 
        //create buffer - projectile
        VertexArray pl_va; // square vertex array
        VertexBuffer pl_vb(pl_positions, 4 * 2 * sizeof(float)); //vertex buffer

        VertexBufferLayout pl_layout;
        pl_layout.Push<float>(2);
        pl_va.AddBuffer(pl_vb, pl_layout);

        // create index buffer - projectile
        IndexBuffer pl_ib(indices, 6);

        Shader pl_shader("res/shaders/Basic.shader");
        pl_shader.Bind();
        //color uniform
        pl_shader.SetUniform4f("u_Color", 0.9f, 0.3f, 0.8f, 1.0f);

        //unbind everything
        pl_va.Unbind();
        pl_vb.Unbind();
        pl_ib.Unbind();
        pl_shader.Unbind();

        ///// floor
        
        //create vertex array (vao)
        unsigned int fl_vao;
        GLCall(glGenVertexArrays(1, &fl_vao));
        GLCall(glBindVertexArray(fl_vao));
        
        //create buffer - floor
        VertexArray fl_va;
        VertexBuffer fl_vb(fl_positions, 4 * 2 * sizeof(float));

        VertexBufferLayout fl_layout;
        fl_layout.Push<float>(2);
        fl_va.AddBuffer(fl_vb, fl_layout);

        // create index buffer - floor
        IndexBuffer fl_ib(fl_indices, 6);

        Shader fl_shader("res/shaders/Basic.shader");
        fl_shader.Bind();
        //color uniform
        fl_shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        //unbind everything
        fl_va.Unbind();
        fl_vb.Unbind();
        fl_ib.Unbind();
        fl_shader.Unbind();

        /////

        float r = 0.0f;
        float increment = 0.005f;
        bool grav_on = false;

        //time - fps counter
        double prevTime = 0.0;
        double crntTime = 0.0;
        double del_time;
        unsigned int counter = 0;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            // userinput
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



            //calculate fps
            crntTime = glfwGetTime(); // get change in time
            del_time = crntTime - prevTime;
         
            counter++;
            if (del_time >= 1.0 / 5.0)
            {
                std::string FPS = std::to_string((1.0 / del_time) * counter);
                std::string ms = std::to_string((del_time / counter) * 1000);
                std::string newTitle = FPS + "FPS / " + ms + "ms";
                glfwSetWindowTitle(window, newTitle.c_str());
                prevTime = crntTime;
                counter = 0;
            }

            del_time = 0.2;
            // Animation:
            std::vector<float> del_position = updatePos(v_velocity, h_velocity, del_time, grav_on);
            v_velocity = del_position[2];

            for (int i = 0; i < (sizeof(positions) / sizeof(float)); i++)
            {
                if (!(i % 2))
                    positions[i] += del_position[0]; //move x coordinate
                else if (i % 2)
                    positions[i] += del_position[1]; //move y coordinate
            }

            //create buffer - square
            VertexArray va;
            VertexBuffer vb(positions, 4 * 2 * sizeof(float));
            VertexBufferLayout layout;
            layout.Push<float>(2);
            va.AddBuffer(vb, layout);

            //bind
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 1.0f, 1.0f);
            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            //create array and buffer - platform
            VertexArray pl_va;
            VertexBuffer pl_vb(pl_positions, 4 * 2 * sizeof(float));
            VertexBufferLayout pl_layout;
            pl_layout.Push<float>(2);
            pl_va.AddBuffer(pl_vb, pl_layout);

            //bind shader
            pl_shader.Bind();
            pl_shader.SetUniform4f("u_Color", 0.3f, 0.1f, 0.3f, 1.0f);
            pl_va.Bind();
            pl_ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            //create array and buffer - floor
            VertexArray fl_va;
            VertexBuffer fl_vb(fl_positions, 4 * 2 * sizeof(float));
            VertexBufferLayout fl_layout;
            fl_layout.Push<float>(2);
            fl_va.AddBuffer(fl_vb, fl_layout);

            //bind shader
            fl_shader.Bind();
            fl_shader.SetUniform4f("u_Color", 0.3f, 0.1f, 0.3f, 1.0f);
            fl_va.Bind();
            fl_ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            //unbind everything
            fl_va.Unbind();
            fl_vb.Unbind();
            fl_ib.Unbind();
            fl_shader.Unbind();

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
#include "window.h"
#include "physics.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



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
    glm::vec2 velocity(0.0f, 0.0f);
    glm::vec2 offset_position(0.0f, 0.0f);
    bool object_stationary = true;

    if (object_stationary)
    {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
    }
    else if (!object_stationary)
    {
        velocity.x = 0.01f;
        velocity.y = 0.01f;
    }

    { 
        //square projectile
        // As square, define positions from origin and length of side
        float x_origin = -1.45f; float y_origin = 0.001f; float length = 0.1f;
        glm::mat4 collisionresolutionMatrix = glm::mat4(1.0f);

        AABB init_squareAABB = {
        glm::vec2(x_origin, y_origin),
        glm::vec2(x_origin + length, y_origin + length)}; //define AABB for square
        AABB squareAABB = init_squareAABB;

        //define initial square vertex positions
        float positions[] = {
                x_origin,           y_origin,                    //index 0 - bottom left (origin: x,y)
                x_origin + length,  y_origin,                    //index 1 - bottom right 
                x_origin + length,  y_origin + length,           //index 2 - top right
                x_origin,           y_origin + length            //index 3 - top left
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        //platform pos - same top height as bottom of projectile, bottom height -1.0f
        const float bottom_screen = -1.5f, left_screen = -2.0f, right_screen = 2.0f, width = 0.8f, height = 1.45f; //define platform relative to screensize and height/width
        AABB pl_AABB = { glm::vec2(left_screen, bottom_screen), glm::vec2(left_screen + width, bottom_screen + height) }; //define AABB for platform

        float pl_positions[] = {
                 left_screen, bottom_screen,                     //index 0 - bottom left (origin: x,y)
                 left_screen + width, bottom_screen,             //index 1 - bottom right 
                 left_screen + width, bottom_screen + height,    //index 2 - top right
                 left_screen, bottom_screen + height             //index 3 - top left
        };

        //floor 
        float fl_height = 0.5f;
        AABB fl_AABB = { glm::vec2(left_screen, bottom_screen), glm::vec2(right_screen, bottom_screen + fl_height) }; //define AABB for platform
        float fl_positions[] = {
                left_screen,    bottom_screen,                   //index 0 - bottom left (origin: x,y)
                right_screen,   bottom_screen,                   //index 1 - bottom right 
                right_screen,   bottom_screen + fl_height,       //index 2 - top right
                left_screen,    bottom_screen + fl_height        //index 3 - top left
        };
        unsigned int fl_indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        const float top_screen = 1.5f;
        glm::mat4 proj = glm::ortho(left_screen, right_screen, bottom_screen, top_screen, 0.0f, 1.0f); //projection matrix
        
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
        shader.SetUniformMat4f("u_MVP", proj);
        shader.SetUniformMat4f("collisionresolutionMatrix", collisionresolutionMatrix);

        //unbind everything
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        ///// platform
        // 
        //create buffer - platform
        VertexArray pl_va; // square vertex array
        VertexBuffer pl_vb(pl_positions, 4 * 2 * sizeof(float)); //vertex buffer

        VertexBufferLayout pl_layout;
        pl_layout.Push<float>(2);
        pl_va.AddBuffer(pl_vb, pl_layout);

        // create index buffer - platform
        IndexBuffer pl_ib(indices, 6);

        Shader pl_shader("res/shaders/Basic.shader");
        pl_shader.Bind();
        //color uniform
        pl_shader.SetUniform4f("u_Color", 0.9f, 0.3f, 0.8f, 1.0f);
        pl_shader.SetUniformMat4f("u_MVP", proj);

        //unbind everything
        pl_va.Unbind();
        pl_vb.Unbind();
        pl_ib.Unbind();
        pl_shader.Unbind();


        ///// floor
        //
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
        fl_shader.SetUniformMat4f("u_MVP", proj);

        //unbind everything
        fl_va.Unbind();
        fl_vb.Unbind();
        fl_ib.Unbind();
        fl_shader.Unbind();

        /////

        float r = 0.0f;
        float increment = 0.005f;
        bool grav_on = false;


        //time - fps counter, fixedTimeStep
        double fps_prevTime = glfwGetTime();
        double fps_crntTime = 0.0;
        double fps_frameTime;
        double prevTime = glfwGetTime();
        double crntTime = 0.0;
        double frameTime;
        unsigned int counter = 0;
        glm::mat4 translationMatrix = glm::mat4(1.0f);
        
        //animation timesteps
        const double fixedTimeStep = 1.0 / 10.0; //30 ticks per second
        double accumulator = 0.0;

        //std::cout << "\n___ AABB intial positions: ___" << std::endl;
        //std::cout << "\nSQUARE PROJECTILE: " << std::endl;
        //std::cout << "square: min x -> " << squareAABB.min.x << std::endl;
        //std::cout << "square: min y -> " << squareAABB.min.y << std::endl;
        //std::cout << "square: max x -> " << squareAABB.max.x << std::endl;
        //std::cout << "square: max y -> " << squareAABB.max.y << std::endl;
        //std::cout << "\nPLATFORM: " << std::endl;
        //std::cout << "square: min x -> " << pl_AABB.min.x << std::endl;
        //std::cout << "square: min y -> " << pl_AABB.min.y << std::endl;
        //std::cout << "square: max x -> " << pl_AABB.max.x << std::endl;
        //std::cout << "square: max y -> " << pl_AABB.max.y << std::endl;
        //std::cout << "\nFLOOR: " << std::endl;
        //std::cout << "floor: min x -> " << fl_AABB.min.x << std::endl;
        //std::cout << "floor: min y -> " << fl_AABB.min.y << std::endl;
        //std::cout << "floor: max x -> " << fl_AABB.max.x << std::endl;
        //std::cout << "floor: max y -> " << fl_AABB.max.y << std::endl;

        //if (AABBIntersect(squareAABB, fl_AABB)) std::cout << "\nCollsion with floor";
        //if (AABBIntersect(squareAABB, pl_AABB)) std::cout << "\nCollsion with platform";
        //else std::cout << "\nNo initial collsion\n";
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
          
            // userinput
            if ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) && !grav_on)
                grav_on = true;
            else if ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) && grav_on) // ineffective currently
                grav_on = false;

            if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS))
            {
                velocity.x = 0.0015f; velocity.y = 0.0008f; grav_on = true;
            }
            if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS))
            {
                velocity.x = 0.0f; velocity.y = 0.0f; grav_on = false; offset_position = { 0.0f,0.0f };
            }

            // debugger movement. If m held then can fly around 
            if (glfwGetKey(window, GLFW_KEY_M))
            {
                velocity = glm::vec2(0.0f);

                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                    velocity.x = -0.0005f;
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                    velocity.x = 0.0005f;
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    velocity.y = -0.0005f;
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                    velocity.y = 0.0005f;
            }


            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));


            
            //calculate fps
            fps_crntTime = glfwGetTime(); // get change in time
            fps_frameTime = fps_crntTime - fps_prevTime;
            counter++;
            if (fps_frameTime >= 1.0 / 3.0) 
            {
                std::string FPS = std::to_string((1.0 / fps_frameTime) * counter);
                std::string ms = std::to_string((fps_frameTime / counter) * 1000);
                std::string newTitle = FPS + "FPS / " + ms + "ms";
                glfwSetWindowTitle(window, newTitle.c_str());
                fps_prevTime = fps_crntTime;
                counter = 0;
            }

   

            // Animation:
            crntTime = glfwGetTime(); // get change in time
            frameTime = crntTime - prevTime;
            accumulator += frameTime;
            while (accumulator >= fixedTimeStep) 
            {
                // check for collsion
                if ((AABBIntersect(squareAABB, fl_AABB)) || (AABBIntersect(squareAABB, pl_AABB))) {
                    std::cout << "Collision occured" << std::endl; velocity.y = { 0.0f }; offset_position.y += 0.001; // stop movement if collision found
                }
                //std::cout << velocity.y;
                glm::vec2 del_position = updatePos(velocity, fixedTimeStep, grav_on);
                offset_position.x += del_position.x; offset_position.y += del_position.y; // add change in position to current offset from origin 
                velocity.x = del_position.x / fixedTimeStep;
                velocity.y = del_position.y / fixedTimeStep;
                translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(offset_position, 0.0f)); //translation matrix
                collisionresolutionMatrix = CalculateCollisionResolutionMatrix(squareAABB, fl_AABB);


                //update AABBs
                squareAABB.min = init_squareAABB.min + glm::vec2(translationMatrix[3]); // offset position is equal to initial + offset
                squareAABB.max = init_squareAABB.max + glm::vec2(translationMatrix[3]);



                accumulator -= fixedTimeStep;
            }
            
            //std::cout << " AABB x: " << (squareAABB.min).x << " offset x: " << offset_position.x << std::endl;
            if ((squareAABB.min.x != (init_squareAABB.min.x + offset_position.x)) && (squareAABB.min.y != (init_squareAABB.min.y + + offset_position.y)))
            {
                std::cout << "Error: x and y positions do not match" << std::endl; break;
            }
            else if ((squareAABB.min.x) != (init_squareAABB.min.x + offset_position.x))
            {
                std::cout << "Error: x positions do not match" << std::endl; 
                std::cout << squareAABB.min.x << " != " << init_squareAABB.min.x + offset_position.x << std::endl;
                break;
            }
            else if ((squareAABB.min.y) != (init_squareAABB.min.y + offset_position.y))
            {
                std::cout << "Error: y positions do not match" << std::endl;
                std::cout << squareAABB.min.y << " != " << offset_position.y << std::endl;
                break;
            }
            //if (squareAABB.min.y < pl_AABB.max.y) {
            //    std::cout << "collision platform";
            //    break;
            //}

            //diagnose why platform collision not working


            // test platform 
            //std::cout << "\nPlatform test: \n";
            //std::cout << "Square AABB coordinates: min -> x = " << squareAABB.min.x << " y = " << squareAABB.min.y << " max -> x = " << squareAABB.max.x << " y = " << /squareAABB.max.y/ << std::endl;
            //std::cout << "\nY Tests: \n";
            //if (squareAABB.min.y > pl_AABB.max.y) std::cout << "square.min.y > pl.max.y: " << squareAABB.min.y << " > " << pl_AABB.max.y << std::endl;
            //if (squareAABB.max.y < pl_AABB.min.y) std::cout << "square.max.y < pl.min.y: " << squareAABB.max.y << " < " << pl_AABB.min.y << std::endl;
            //else if (!((squareAABB.min.y > pl_AABB.max.y) || (squareAABB.max.y < pl_AABB.min.y))) std::cout << "Intersection occured regarding y coordinates \n";
            //std::cout << "\nX Tests: \n";
            //if (squareAABB.min.x > pl_AABB.max.x) std::cout << "square.min.x > pl.max.x" << std::endl;
            //if (squareAABB.max.x < pl_AABB.min.x) std::cout << "square.min.x < pl.min.x" << std::endl;
            //else if (!((squareAABB.min. > pl_AABB.max.x) || (squareAABB.max.x < pl_AABB.min.x))) std::cout << "Intersection occured regarding x coordinates \n";
            //std::cout << (AABBIntersect(squareAABB, pl_AABB));

            //break;

            

            //create buffer - square
            VertexArray va;
            VertexBuffer vb(positions, 4 * 2 * sizeof(float));
            VertexBufferLayout layout;
            layout.Push<float>(2);
            va.AddBuffer(vb, layout);

            //bind
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 1.0f, 1.0f);
            shader.SetUniformMat4f("transformationMatrix", translationMatrix);
            shader.SetUniformMat4f("collisionresolutionMatrix", collisionresolutionMatrix);
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
            pl_shader.SetUniformMat4f("transformationMatrix", glm::mat4(1.0f)); // don't transform platform
            pl_va.Bind();
            pl_ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // draw platform

            //create array and buffer - floor
            VertexArray fl_va;
            VertexBuffer fl_vb(fl_positions, 4 * 2 * sizeof(float));
            VertexBufferLayout fl_layout;
            fl_layout.Push<float>(2);
            fl_va.AddBuffer(fl_vb, fl_layout);

            //bind shader
            fl_shader.Bind();
            fl_shader.SetUniform4f("u_Color", 0.3f, 0.1f, 0.3f, 1.0f);
            fl_shader.SetUniformMat4f("transformationMatrix", glm::mat4(1.0f)); // don't transform floor
            fl_va.Bind();
            fl_ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // draw floor

            //unbind everything
            fl_va.Unbind();
            fl_vb.Unbind();
            fl_ib.Unbind();
            fl_shader.Unbind();

            // animate colour
            if (r > 1.0f)
                increment = -0.0005f;
            else if (r < 0.0f)
                increment = 0.0005f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            
            //glfwWaitEventsTimeout(0.016); //approx 60fps
            //std::cout << "\nSQUARE PROJECTILE: " << std::endl;
            //std::cout << "square: min x -> " << squareAABB.min.x << std::endl;
            //std::cout << "square: min y -> " << squareAABB.min.y << std::endl;
            //std::cout << "square: max x -> " << squareAABB.max.x << std::endl;
            //std::cout << "square: max y -> " << squareAABB.max.y << std::endl;
        }
    }
    glfwTerminate();

}
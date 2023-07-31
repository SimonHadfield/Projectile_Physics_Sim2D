#include "window.h"
#include "physics.h"

static unsigned int CompileShader(unsigned int type,const std::string& source) 
{
    unsigned int id = glCreateShader(GL_VERTEX_SHADER);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int CreateWindow(bool displayState) {
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
    std::vector<float> anchor_position = { 0.1f,0.1f }; //initialize triangle position x y
    float v_velocity = 0.01f;
    float h_velocity = 0.01f;

    float positions[6] = {
            -0.5f, -0.5f,
             0.0f,  0.5f,
             0.5f, -0.5f
    };
    
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    //unsigned int shader = CreateShader();

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //move anchor
        updatePos(anchor_position, v_velocity, h_velocity);
        //reset anchor if exceeds 1.0f
        if (anchor_position[0] > 1.0f || anchor_position[1] > 1.0f)
        {
            anchor_position[0] = 0.0f;
            anchor_position[1] = 0.0f;
        }
        
        //move vertices by anchor_shift
        positions[0] = -0.5f + anchor_position[0];
        positions[1] = -0.5f + anchor_position[1];
        positions[2] = 0.0f + anchor_position[0];
        positions[3] = 0.5f + anchor_position[1];
        positions[4] = 0.5f + anchor_position[0];
        positions[5] = -0.5f + anchor_position[1];


        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
        //glfwWaitEventsTimeout(0.016); //approx 60fps
    }

    glfwTerminate();

}
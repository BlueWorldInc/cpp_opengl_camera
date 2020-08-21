#include <app.h>

const GLchar* vertexShaderSource = R"END(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    attribute vec3 position;

    void main()
    {
        gl_Position = projection * model * view * vec4(position, 1.0);
    }
)END";

// gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

const GLchar* fragmentShaderSource = R"END(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    } 
)END";

int main(void) {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "3D FPS View", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();


    // VBO setup

    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //      0.5f, -0.5f, 0.0f,
    //      0.0f,  0.5f, 0.0f
    // };

    GLfloat vertices[] = {
        -1, -1, +1,
        -1, +1, +1,
        +1, +1, +1,
        +1, -1, +1,
        -1, -1, -1,
        -1, +1, -1,
        +1, +1, -1,
        +1, -1, -1,
    };

    GLfloat colors[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 1,
        1, 1, 0,
        0, 1, 1,
        0, 1, 0,
        1, 0, 0,
    };

    GLubyte indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 5,
        0, 5, 1,
        1, 5, 6,
        1, 6, 2,
        3, 2, 6,
        3, 6, 7,
        4, 0, 7,
        7, 0, 3,
        7, 6, 5,
        7, 5, 4,
    };

    GLuint VBO;
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint verticesBuf;
    glGenBuffers(1, &verticesBuf);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint colorsBuf;
    glGenBuffers(1, &colorsBuf);
    glBindBuffer(GL_ARRAY_BUFFER, colorsBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint indicesBuf;
    glGenBuffers(1, &indicesBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);  

    // vertex shader

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment shader

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // shader program

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    // attributes

    GLuint attribPosition;
    attribPosition = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(attribPosition);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuf);
    glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint attribColor;
    attribColor = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(attribColor);
    glBindBuffer(GL_ARRAY_BUFFER, colorsBuf);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        GLint uniformModel = glGetUniformLocation(shaderProgram, "model");
        GLint uniformView  = glGetUniformLocation(shaderProgram, "view");
        GLint uniformProjection = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        usleep(20000);
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
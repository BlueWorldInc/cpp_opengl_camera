#include <app.h>

const GLchar* vertex120 = R"END(
#version 330 core
layout (location = 0) in vec3 aPos;
attribute vec3 position;
attribute vec3 color;
attribute vec2 inUvs;
varying vec3 outColor;
varying vec2 outUvs;
uniform float time;
uniform mat4 matrix;
uniform mat4 projection;
uniform mat4 camera;
void main()
{
    outColor = color;
    outUvs = inUvs;
    gl_Position = projection * matrix * camera * vec4(position, 1.f);
}
)END";

const GLchar* raster120 = R"END(
#version 120
varying vec3 outColor;
varying vec2 outUvs;
uniform sampler2D tex; // 1st texture slot by default
uniform float time;
void main()
{
    gl_FragColor = vec4(texture2D(tex, outUvs)/2.f + vec4(outColor,1.f)/2.f);
}
)END";

int main(void) {

    

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Shaders", NULL, NULL);



    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    const char* source;

    // vertex shader

    source = vertex120;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &source, 0);
    glCompileShader(vertexShader);
    GLint compilationStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationStatus);

    if (compilationStatus == GL_FALSE) {
        char message[256];
        glGetShaderInfoLog(vertexShader, sizeof(message), 0, &message[0]);
        printf("%s", message);
        exit(1);
    }

    
    // fragment shader

    source = raster120;

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &source, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);

    if (compilationStatus == GL_FALSE) {
        char message[256];
        glGetShaderInfoLog(fragmentShader, sizeof(message), 0, &message[0]);
        printf("%s", message);
        exit(1);
    }

    // shader program

    GLint linkStatus;
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compilationStatus);

    if (compilationStatus == GL_FALSE) {
        GLchar message[256];
        glGetProgramInfoLog(shaderProgram, sizeof(message), 0, &message[0]);
        printf("%d", message);
        exit(1);
    }

    glUseProgram(shaderProgram);

    // VBO setup

    GLfloat vertices[] = {
        -1, -1, +1,
        -1, +1, +1,
        +1, +1, +1,
        +1, -1, +1,
        -1, -1, -1,
        -1, +1, -1,
        +1, +1, -1,
        +1, -1, -1,
        -1, -1, +1, // 0
        -1, +1, +1,
        +1, +1, +1,
        +1, -1, +1,
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
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
    };

    GLubyte indices[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 1,
        1, 0,
        1, 5,
        5, 6,
        6, 2,
        2, 1,
        3, 2,
        2, 6,
        6, 7,
        7, 3,
        7, 6,
        6, 5,
        5, 4,
        4, 7,
        0, 3,
        3, 7,
        7, 4,
        4, 0,
    };

    GLuint verticesBuf;
    glGenBuffers(1, & verticesBuf);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint colorsBuf;
    glGenBuffers(1, & colorsBuf);
    glBindBuffer(GL_ARRAY_BUFFER, colorsBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint indicesBuf;
    glGenBuffers(1, & indicesBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


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
    
    GLfloat matrix[] = {
        0.5, 0,   0,   0,
        0,   0.5, 0,   0,
        0,   0,   0.5, 0,
        0,   0,   0,   1
    };
    
    GLuint attribMatrix;
    attribMatrix = glGetUniformLocation(shaderProgram, "matrix");
    glUniformMatrix4fv(attribMatrix, 1, GL_FALSE, matrix);

    GLuint uniformTime;
    uniformTime = glGetUniformLocation(shaderProgram, "time");

    glm::mat4 projectionMatrix = glm::mat4(1.f);// glm::perspective(glm::radians(60.f), 1.f, 0.f, 10.f);
    GLint uniformProjection = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    // tex
    
    bmpread_t bitmap;
    
    if (!bmpread("media/texture.bmp",0, &bitmap)) {
        printf("Texture loading error");
        exit(-1);
    }
    
    GLuint texid;
    glGenTextures(1, &texid);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texid);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage2D(GL_TEXTURE_2D,0,3,bitmap.width,bitmap.height,0,GL_RGB,GL_UNSIGNED_BYTE,bitmap.data);
    
    GLuint attribTex = glGetAttribLocation(shaderProgram, "tex");
    glUniform1i(attribTex, 0);


    // uvs

    GLfloat uvs[] = {
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 1,
        1, 1,
        1, 0,
    };
    
    GLuint uvsData;
    glGenBuffers(1, &uvsData);
    glBindBuffer(GL_ARRAY_BUFFER, uvsData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    
    GLuint attribUvs;
    attribUvs = glGetAttribLocation(shaderProgram, "inUvs");
    glEnableVertexAttribArray(attribUvs);
    glBindBuffer(GL_ARRAY_BUFFER, uvsData);
    glVertexAttribPointer(attribUvs, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // glEnable(GL_CULL_FACE);

    glLineWidth(2);

    glm::mat4 scaleMatrix = glm::mat4(1.f);
    scaleMatrix = glm::translate(scaleMatrix, glm::vec3(0, 0, -2));

    glm::mat4 projMatrix = glm::perspective(glm::radians(60.f), 1.f, 0.f, 10.f) * scaleMatrix;
    GLint uniformProj = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(projMatrix));

    // camera

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    const float cameraSpeed = 0.05f; // adjust accordingly

    // Camera cam(glm::vec3(0.0f, 0.0f,  3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f));
    // Camera cam(cameraPos, cameraPos, cameraPos, cameraPos);

    Camera cam(cameraPos, cameraFront, cameraUp, cameraSpeed);
    Camera* cam_ptr = &cam;

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(upVector, cameraDirection));
    glm::vec3 up = glm::cross(cameraDirection, cameraRight);
    glm::mat4 camera = glm::mat4(1.0f);

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    camera = glm::lookAt(cam.cameraPos, cameraPos + cameraFront, up);
    
    GLint uniformCam = glGetUniformLocation(shaderProgram, "camera");
    glUniformMatrix4fv(uniformCam, 1, GL_FALSE, glm::value_ptr(camera));

    const float radius = 1.0f;

    // look around

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    float lastX = 400, lastY = 300;
    float sensitivity = 0.1f;

    Mouse mouse(lastX, lastY, sensitivity);
    
    glm::vec3 direction;
    float yaw = -90.0f;
    float pitch = 0.0f;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        (*cam_ptr).cameraSpeed = 2.5f * deltaTime;

        glfwSetCursorPosCallback(window, mouse_move_callback);
        glfwSetScrollCallback(window, scroll_callback); 

        mouse_callback(window, &mouse, &cam, &yaw, &pitch);

        glm::mat4 projMatrix = glm::perspective(glm::radians((*cam_ptr).zoom), 1.f, 0.f, 10.f) * scaleMatrix;
        GLint uniformProj = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(projMatrix));

        processInput(window, cam_ptr);

        // printf("%d\n", cam.a);

        // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //     cameraPos += cameraSpeed * cameraFront;
        // }

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        // glUniform1f(uniformTime, time);
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        camera = glm::lookAt(cam.cameraPos, cam.cameraPos + cam.cameraFront, cam.cameraUp);
        
        glUniformMatrix4fv(uniformCam, 1, GL_FALSE, glm::value_ptr(camera));

        glDrawElements(GL_LINES, sizeof(indices), GL_UNSIGNED_BYTE, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        usleep(20000);
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, Camera* camera)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        (*camera).cameraPos += (*camera).cameraSpeed * (*camera).cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        (*camera).cameraPos -= (*camera).cameraSpeed * (*camera).cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        (*camera).cameraPos -= glm::normalize(glm::cross((*camera).cameraFront, (*camera).cameraUp)) * (*camera).cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        (*camera).cameraPos += glm::normalize(glm::cross((*camera).cameraFront, (*camera).cameraUp)) * (*camera).cameraSpeed;
        std::cout << glm::to_string((*camera).cameraPos) << std::endl;
    }
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
    _xPos = xpos;
    _yPos = ypos;
    _mouse_moved = true;
   
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    _camera_zoom -= (float)yoffset;
    if (_camera_zoom < 1.0f)
        _camera_zoom = 1.0f;
    if (_camera_zoom > 90.0f)
        _camera_zoom = 90.0f; 
    _zoomed = true;
}

void mouse_callback(GLFWwindow* window, Mouse* mouse, Camera* camera, float* yaw, float* pitch) {

    if (_mouse_moved) {

    (*mouse).xPos = _xPos;
    (*mouse).yPos = _yPos;

    if ((*mouse).firstTime) {
        (*mouse).lastX = (*mouse).xPos;
        (*mouse).lastY = (*mouse).yPos;
        (*mouse).firstTime = false;
        printf("hello\n");
    }
  
    float xoffset = (*mouse).xPos - (*mouse).lastX;
    float yoffset = (*mouse).lastY - (*mouse).yPos; 

    printf("----\n");
    printf("%f\n", (*mouse).xPos);
    printf("%f\n", (*mouse).yPos);
    printf("%f\n", (*mouse).lastX);
    printf("%f\n", (*mouse).lastY);

    (*mouse).lastX = (*mouse).xPos;
    (*mouse).lastY = (*mouse).yPos;
    xoffset *= (*mouse).sensitivity;
    yoffset *= (*mouse).sensitivity;

    (*yaw)   += xoffset;
    (*pitch) += yoffset;

    if((*pitch) > 89.0f)
        (*pitch) = 89.0f;
    if((*pitch) < -89.0f)
        (*pitch) = -89.0f;


    glm::vec3 direction;
    direction.x = cos(glm::radians((*yaw))) * cos(glm::radians((*pitch)));
    direction.y = sin(glm::radians((*pitch)));
    direction.z = sin(glm::radians((*yaw))) * cos(glm::radians((*pitch)));
    (*camera).cameraFront = glm::normalize(direction);

    std::cout << glm::to_string((*camera).cameraFront) << std::endl;

    _mouse_moved = false;
    } else if (_zoomed) {
        (*camera).zoom = _camera_zoom;
        _zoomed = false;
    }

}
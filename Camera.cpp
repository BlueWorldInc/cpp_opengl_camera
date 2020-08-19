class Camera 
{
public:
    Camera(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 upVector)
    {
        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
        glm::vec3 cameraRight = glm::normalize(glm::cross(upVector, cameraDirection));
        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
        glm::mat4 camera = glm::mat4(1.0f);

        GLint uniformCam = glGetUniformLocation(shaderProgram, "camera");
        glUniformMatrix4fv(uniformCam, 1, GL_FALSE, glm::value_ptr(camera));
    }
}
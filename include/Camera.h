class Camera {
public:
    Camera (glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float cameraSpeed) {
        this->cameraPos = cameraPos;
        this->cameraFront = cameraFront;
        this->cameraUp = cameraUp;
        this->cameraSpeed = cameraSpeed;
    }
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;
};
class Camera {
public:
    Camera (glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float cameraSpeed) {
        this->cameraPos = cameraPos;
        this->cameraFront = cameraFront;
        this->cameraUp = cameraUp;
        this->cameraSpeed = cameraSpeed;
        this->zoom = 60.0f;
    }
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;
    float zoom;
};
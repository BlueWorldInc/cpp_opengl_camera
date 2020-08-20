class Mouse {
public:
    Mouse (double xPos, double yPos, float sensitivity) {
        this->xPos = xPos;
        this->yPos = yPos;
        this->sensitivity = sensitivity;
        this->firstTime = true;
    }
    bool firstTime;
    double xPos;
    double yPos;
    double lastX;
    double lastY;
    float sensitivity;
};
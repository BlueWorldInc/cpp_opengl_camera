#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <bmpread.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera.h>
#include <mouse.h>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#define GL_SILENCE_DEPRECATION 1

const unsigned int steps = 100;
const double stepAngle = 3.1415926f * 2.f / steps;

float _xPos;
float _yPos;
float _mouse_moved = false;
float _zoomed = false;
float _camera_zoom = 60.0f;

void drawCircle(double red, double green, double blue);
void drawPlanet(double angle, double distance, double diameter, double red, double green, double blue);
void processInput(GLFWwindow* window, Camera* camera);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_callback(GLFWwindow* window, Mouse* mouse, Camera* camera, float* yaw, float* pitch);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
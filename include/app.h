#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <bmpread.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GL_SILENCE_DEPRECATION 1

const unsigned int steps = 100;
const double stepAngle = 3.1415926f * 2.f / steps;

void drawCircle(double red, double green, double blue);
void drawPlanet(double angle, double distance, double diameter, double red, double green, double blue);
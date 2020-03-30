#include "Global.hpp"
//#include "Object.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;



GLFWwindow* Global::window = nullptr;

int Global::windowX = 1800;
int Global::windowY = 960;
float Global::screenProportions = (float)Global::windowX / (float)Global::windowY;

void Global::windowSizeCallback(GLFWwindow* window, int width, int height)
{
	
	windowY = height;
	windowX = width;
	glViewport(0, 0, windowX, windowY);
	screenProportions = (float)windowY / (float)windowX;
}


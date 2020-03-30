#pragma once
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
using namespace glm;

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

class Global
{
public:
	static struct GLFWwindow* window;
	static int windowX, windowY;
	static float screenProportions;

	static void windowSizeCallback(GLFWwindow* window, int width, int height);
	
	template <class T>
	static void deleteVector(std::vector<T*>* vectorToDelete)
	{
		for (unsigned int i = 0; i < vectorToDelete->size(); i++)
		{
			delete vectorToDelete->at(i);
		}
	}


};

#endif
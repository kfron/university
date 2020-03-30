#pragma once
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
using namespace glm;

#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object
{
protected:
	vec3 position;
	vec3 rotation;
	vec3 scale;
public:
	std::string name;
	virtual vec3 getPosition()
	{
		return position;
	}
	virtual vec3 getRotation()
	{
		return rotation;
	}
	virtual vec3 getScale()
	{
		return scale;
	}

	virtual void setPosition(vec3 newPos)
	{
		position = newPos;
	}
	virtual void setRotation(vec3 newRotation)
	{
		rotation = newRotation;
	}
	virtual void setScale(vec3 newScale)
	{
		scale = newScale;
	}

	virtual void update(float deltaTime) {};
	//virtual void afterSpawn() {};

	Object();
	virtual ~Object();
};

#endif
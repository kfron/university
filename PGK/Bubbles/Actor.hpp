#pragma once
#include "Object.hpp"
#include "Component.hpp"

#ifndef ACTOR_HPP
#define ACTOR_HPP

class Actor :
	public Object
{
protected:
	std::vector<Component*> components;
public:
	Actor();

	virtual void update(float deltaTime) override;
	virtual void setPosition(vec3 newPos) override
	{
		position = newPos;
	}
	virtual void setRotation(vec3 newRotation)override
	{
		rotation = newRotation;
	}
	virtual void setScale(vec3 newScale)override
	{
		scale = newScale;
	}

	virtual ~Actor();
};

#endif
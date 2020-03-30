#pragma once
#include "Object.hpp"

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class Component :
	public Object
{
protected:
	class Actor* owner;
public:
	Component(class Actor* Owner)
	{
		owner = Owner;
	};

	virtual void update(float deltaTime) override;
	virtual ~Component();
};


#endif
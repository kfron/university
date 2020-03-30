#include "Component.hpp"
#include "Actor.hpp"
#include <iostream>




void Component::update(float deltaTime)
{
	position = owner->getPosition();
	rotation = owner->getRotation();
	scale = owner->getScale();
}

Component::~Component()
{
}

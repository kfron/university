#include "Actor.hpp"
#include "Global.hpp"
#include <iostream>

Actor::Actor()
{

}

void Actor::update(float deltaTime)
{
	for(unsigned int i = 0; i < components.size(); i++){
		components[i]->update(deltaTime);
	}
}

Actor::~Actor()
{
	Global::deleteVector<Component>(&components);
}
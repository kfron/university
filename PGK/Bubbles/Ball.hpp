#pragma once
#include "Actor.hpp"

#ifndef BALL_HPP
#define BALL_HPP

class Ball :
	public Actor
{
public:
	Ball();
	Ball(vec3 pos, vec3 rot, unsigned int shaderID);
	~Ball();
	
};

#endif
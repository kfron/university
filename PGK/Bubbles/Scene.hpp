#pragma once
#include <vector>
#include "Actor.hpp"

#ifndef SCENE_HPP
#define SCENE_HPP

class Scene
{
	public:
		std::vector<Actor*> actors;
		Scene(unsigned int shaderID);
		~Scene();
};

#endif
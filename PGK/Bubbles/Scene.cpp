#include "Scene.hpp"
#include "Global.hpp"
#include "Ball.hpp"

Scene::Scene(unsigned int shaderID)
{
	int max = 20;
	for(int i = -max; i < max; i++)
	{
		for(int j = -max; j < max; j++)
		{
			actors.push_back(new Ball(vec3(1.4 * i, -3, 1.3 * j), vec3(0), shaderID));
			actors.back()->setScale(vec3(1, 1.5, 1));
		}
	}
}

Scene::~Scene()
{
	Global::deleteVector<Actor>(&actors);
}
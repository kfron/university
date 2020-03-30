#include "Ball.hpp"
#include "MeshComponent.hpp"
#include "Mesh.hpp"


Ball::Ball()
{
}

Ball::Ball(vec3 pos, vec3 rot, unsigned int shaderID)
{

	MeshComponent* mesh = new MeshComponent(this, &Meshes::sphere ,shaderID);
	components.push_back(mesh);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(1));
	
	
}


Ball::~Ball()
{
}

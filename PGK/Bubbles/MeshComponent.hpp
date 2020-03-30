#pragma once
#include "Component.hpp"

#ifndef MESHCOMPONENT_HPP
#define MESHCOMPONENT_HPP

//Loading and rendering of .obj files
class MeshComponent :
	public Component
{
private:
	class Mesh* mesh;

	mat4 transMatrix;// = translate(glm::mat4(), vec3(1.0f, 0.0f, 0.0f));
	mat4 scalingMatrix; //= scale(vec3(1, 1, 1));
	mat4 rotationMatrix;//= rotate(30.0f, vec3(0, 0, 1));
	vec4 diffuseColor;

	unsigned int shaderID;
	
public:
	MeshComponent(class Actor* Owner, class Mesh* Mesh, unsigned int ShaderID, vec4 colorOverride = vec4(0));
	
	void update(float DeltaTime);

	virtual ~MeshComponent();
};



#endif
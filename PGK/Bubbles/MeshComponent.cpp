#include "MeshComponent.hpp"
#include "Actor.hpp"
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"
#include "Controls.hpp"
#include <iostream>
#include "Mesh.hpp"



MeshComponent::MeshComponent(class Actor* Owner, Mesh* Mesh,unsigned int ShaderID, vec4 colorOverride)
	:
	Component(Owner),
	shaderID(ShaderID),
	mesh(Mesh)
{
	if (colorOverride == vec4(0))
		diffuseColor = mesh->diffuseColor;
	else
		diffuseColor = colorOverride;
}



void MeshComponent::update(float DeltaTime)
{
	Component::update(DeltaTime);
	transMatrix = translate(mat4(), owner->getPosition()+getPosition());

	scalingMatrix = glm::scale(owner->getScale()*getScale());

	rotationMatrix = rotate(rotation.x + owner->getRotation().x, vec3(1, 0, 0));
	rotationMatrix = rotate(rotationMatrix, rotation.y + owner->getRotation().y, vec3(0, 1, 0));
	rotationMatrix = rotate(rotationMatrix, rotation.z + owner->getRotation().z, vec3(0, 0, 1));

	mat4 ModelMatrix = transMatrix * rotationMatrix * scalingMatrix;
	mat4 ProjectionMatrix = getProjectionMatrix();
	mat4 ViewMatrix = getViewMatrix();

	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	GLuint shader = shaderID;
	GLuint colorID = glGetUniformLocation(shader, "diffuseColor");
	GLuint LightID = glGetUniformLocation(shader, "LightPosition_worldspace");
	GLuint MatrixID = glGetUniformLocation(shader, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(shader, "V");
	GLuint ModelMatrixID = glGetUniformLocation(shader, "M");

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

	vec3 lightPos = glm::vec3(4, 4, 4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	
	
	glUniform4f(colorID, diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
	
	

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->elementbuffer);

	std::cout << "rysowanko" << std::endl;

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_SHORT, (void*)0);

}


MeshComponent::~MeshComponent()
{
}

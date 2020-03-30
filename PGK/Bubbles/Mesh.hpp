#pragma once
#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#ifndef MESH_HPP
#define MESH_HPP

class Mesh
{
	
public:
	Mesh();
	Mesh(const char* objPath);
	glm::vec4 diffuseColor;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	void deleteBuffers();
	~Mesh();
};

//just a place to hold and get loaded meshes
class Meshes
{
public:
	//call this in main before loop to load meshes
	std::vector<Mesh*> loadedMeshes;
	void loadMeshes();
	static Mesh sphere;

	~Meshes();
};

#endif
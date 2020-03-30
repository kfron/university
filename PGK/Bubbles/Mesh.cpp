#include "Mesh.hpp"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"
#include <iostream>


Mesh::Mesh()
{
}

Mesh::Mesh(const char* objPath)
{

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	if (loadOBJ(objPath, vertices, uvs, normals))
	{
		
		indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	
	}
	else
	{
		std::cout << "failed to load " + *objPath << std::endl;
	}
}


void Mesh::deleteBuffers()
{

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

Mesh::~Mesh()
{

}


Mesh Meshes::sphere;

void Meshes::loadMeshes()
{
	sphere = Mesh("models/sphere.obj");
	sphere.diffuseColor = glm::vec4(0.2, 0.4, 1, 0.6);
	loadedMeshes.push_back(&sphere);
}

Meshes::~Meshes()
{
	for (unsigned int i = 0; i < loadedMeshes.size(); i++)
	{
		loadedMeshes[i]->deleteBuffers();
	}
}

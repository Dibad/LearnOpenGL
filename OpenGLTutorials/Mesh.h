#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad\glad.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Shader.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};


struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh
{
private:
	GLuint VAO_, VBO_, EBO_;

	std::vector<Vertex> vertices_;
	std::vector<Texture> textures_;

public:
	std::vector<GLuint> indices_;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

	GLuint getVAO() const;

	void draw(const Shader & shader);

private:
	void setupMesh();
};


#pragma once

#include <glad\glad.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "stb_image.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <iostream>
#include <string>
#include <vector>

#include "Mesh.h"
#include "Shader.h"

class Model
{
private:
	std::vector<Mesh> meshes_;
	std::string directory_;

	std::vector<Texture> textures_loaded_;

	bool gammaCorrection;

public:
	Model(const std::string & path, bool gamma = false);

	void draw(const Shader & shader);

	static GLuint textureFromFile(const char * path, const std::string & directory = " ", bool gamma = false);

	void info() const;

private:
	void loadModel(const std::string & path);
	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
};


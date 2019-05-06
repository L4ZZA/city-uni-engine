#pragma once

#include "mesh.h"
#include "shader.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace engine
{
	class model
	{
	public:
		model(const std::string& path);
		~model();

		void render(shader& shader);

	private: // methods
		void process_node(aiNode* node, const aiScene* scene);
		mesh process_mesh(aiMesh* mesh, const aiScene* scene);
		std::vector<texture> load_textures(aiMaterial* mat, aiTextureType type, std::string typeName) const;

	private: // fields
		std::vector<mesh> m_meshes;
		std::string m_directory;
		std::string m_path;
	};

}


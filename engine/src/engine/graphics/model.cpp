#include "pch.h"
#include "model.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "texture.h"

engine::model::model(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if((!scene) || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode))
	{
		ENGINE_CORE_ERROR("[ASSIMP] Unable to load model '{0}'.", path);
		return;
	}
	m_directory = path.substr(0, path.find_last_of('/') + 1);

	process_node(scene->mRootNode, scene);
}

engine::model::~model()
{}

void engine::model::render(shader& shader)
{
	for(auto& m_mesh : m_meshes)
		m_mesh.render(shader);
}

void engine::model::process_node(aiNode * node, const aiScene * scene)
{
	for(uint32 i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(process_mesh(mesh, scene));
	}

	for(uint32 i = 0; i < node->mNumChildren; i++)
	{
		process_node(node->mChildren[i], scene);
	}

}

engine::mesh engine::model::process_mesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<mesh::vertex> vertices;
	std::vector<uint32> indices;
	std::vector<texture> textures;

	// == Process vertices
	for(uint32 i = 0; i < mesh->mNumVertices; i++)
	{
		mesh::vertex vert;

		// Position
		glm::vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vert.position = pos;

		// Normal
		glm::vec3 norm(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vert.normal = norm;

		// TexCoords
		if(mesh->mTextureCoords[0])
		{// Does it have any texture coordinates?
			glm::vec2 tex(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			vert.tex_coords = tex;
		}
		else
		{
			vert.tex_coords = glm::vec2(0.0f, 0.0f);
		}

		// Push into vertex array
		vertices.push_back(vert);
	}

	// == Process indices
	for(uint32 i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(uint32 j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// == Process materials
	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<texture> diffuseMaps = load_textures(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<texture> specularMaps = load_textures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return engine::mesh(vertices, indices, textures);
}

std::vector<engine::texture> engine::model::load_textures(aiMaterial * mat, aiTextureType type, std::string typeName) const
{
	std::vector<texture> textures;
	for(uint32 i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString filename;
		mat->GetTexture(type, i, &filename);

		std::string fullpath = m_directory + std::string(filename.C_Str());

		texture texture(fullpath, typeName);
		//texture.LoadTexture();
		textures.push_back(texture);
	}

	return textures;
}

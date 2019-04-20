#include "pyro_pch.h"
#include "model.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "texture.h"

namespace pyro::graphics 
{

  Model::Model(const std::string& path)
  {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if ((!scene) || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode)) {
      PYRO_CORE_ERROR("[ASSIMP] Unable to load model '{0}'.", path);
      return;
    }
    this->directory = path.substr(0, path.find_last_of('/') + 1);

    processNode(scene->mRootNode, scene);
  }

  Model::~Model()
  {
  }

  void Model::Render(Shader& shader)
  {
    for (uint32 i = 0; i < this->meshes.size(); i++)
      this->meshes[i].Render(shader);
  }

  void Model::processNode(aiNode * node, const aiScene * scene)
  {
    for (uint32 i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }

    for (uint32 i = 0; i < node->mNumChildren; i++) {
      this->processNode(node->mChildren[i], scene);
    }

  }
  
  Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
  {
    std::vector<Mesh::Vertex> vertices;
    std::vector<uint32> indices;
    std::vector<texture> textures;

    // == Process vertices
    for (uint32 i = 0; i < mesh->mNumVertices; i++) {
      Mesh::Vertex vert;

      // Position
      glm::vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
      vert.Position = pos;

      // Normal
      glm::vec3 norm(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
      vert.Normal = norm;

      // TexCoords
      if (mesh->mTextureCoords[0]) {// Does it have any texture coordinates?
        glm::vec2 tex(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        vert.TexCoords = tex;
      }
      else {
        vert.TexCoords = glm::vec2(0.0f, 0.0f);
      }

      // Push into vertex array
      vertices.push_back(vert);
    }

    // == Process indices
    for (uint32 i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (uint32 j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    // == Process materials
    if (mesh->mMaterialIndex >= 0)
    {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      std::vector<texture> diffuseMaps = this->loadTextures(material, aiTextureType_DIFFUSE, "diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

      std::vector<texture> specularMaps = this->loadTextures(material, aiTextureType_SPECULAR, "specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
  }
  
  std::vector<texture> Model::loadTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
  {
    std::vector<texture> textures;
    for (uint32 i = 0; i < mat->GetTextureCount(type); i++) {
      aiString filename;
      mat->GetTexture(type, i, &filename);

      std::string fullpath = this->directory + std::string(filename.C_Str());

      texture texture(fullpath, typeName);
      //texture.LoadTexture();
      textures.push_back(texture);
    }

    return textures;
  }
}

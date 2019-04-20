#pragma once

#include "mesh.h"
#include "shader.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace pyro::graphics 
{

  class Model
  {
  public:
    Model(const std::string& path);
    ~Model();

    void Render(Shader& shader);

  private: // methods
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<texture> loadTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

  private: // fields
    std::vector<Mesh> meshes;
    std::string directory;
  };

}


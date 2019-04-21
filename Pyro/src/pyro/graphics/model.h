#pragma once

#include "mesh.h"
#include "shader.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace pyro 
{
  class model
  {
  public:
    model(const std::string& path);
    ~model();

    void render(shader& shader);

  private: // methods
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<texture> load_textures(aiMaterial* mat, aiTextureType type, std::string typeName);

  private: // fields
    std::vector<Mesh> m_meshes;
    std::string m_directory;
  };

}


#pragma once

#include "texture.h"
#include "shader.h"

namespace pyro 
{

  class Mesh
  {
  public:
    struct Vertex 
  	{
      glm::vec3 Position;
      glm::vec3 Normal;
      glm::vec2 TexCoords;
    };

  public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const std::vector<texture>& textures);
    ~Mesh();

    void Render(pyro::shader& shader);

  private:
    std::vector<Vertex>   m_vertices;
    std::vector<uint32>   m_indices;
    std::vector<texture>  m_textures;

    uint32 m_vao{0};
    uint32 m_vbo{0};
    uint32 m_ebo{0};
  };

}


#pragma once

#include "texture.h"
#include "shader.h"

namespace engine 
{
  class mesh
  {
  public:
    struct vertex 
  	{
      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 tex_coords;
    };

  public:
    mesh(const std::vector<vertex>& vertices, const std::vector<uint32>& indices, const std::vector<texture>& textures);
    ~mesh();

    void render(engine::shader& shader);

  private:
    std::vector<vertex>   m_vertices;
    std::vector<uint32>   m_indices;
    std::vector<texture>  m_textures;

    uint32 m_vao{0};
    uint32 m_vbo{0};
    uint32 m_ebo{0};
  };

}


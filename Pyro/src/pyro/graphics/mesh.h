#pragma once

#include "texture.h"
#include "shader.h"

namespace pyro::graphics 
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

    void Render(pyro::graphics::Shader& shader);

  private:
    std::vector<Vertex>   vertices;
    std::vector<uint32>   indices;
    std::vector<texture>  textures;

    uint32 vao;
    uint32 vbo;
    uint32 ebo;
  };

}


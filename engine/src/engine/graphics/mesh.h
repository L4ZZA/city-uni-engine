#pragma once

#include "buffers/vertex_array.h"
#include "buffers/buffer.h"
#include "buffers/index_buffer.h"
#include "texture.h"

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

		//void render(engine::shader& shader);

		const std::vector<texture>& textures() const { return m_textures; }
		const vertex_array& va() const { return m_va; }
		uint32 indices_count() const { return m_indices.size(); }

	private:
		std::vector<vertex>   m_vertices;
		std::vector<uint32>   m_indices;
		std::vector<texture>  m_textures;

		//uint32 m_vao{ 0 };
		uint32 m_vbo{ 0 };
		uint32 m_ebo{ 0 };

		vertex_array m_va{};
		buffer		 m_buffer{};
		index_buffer m_index_buffer{};
	};

}


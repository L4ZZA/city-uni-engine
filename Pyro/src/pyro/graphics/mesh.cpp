#include "pyro_pch.h"
#include "mesh.h"

#include "glad/glad.h"

namespace pyro 
{

	Mesh::Mesh(
		const std::vector<Vertex>& vertices, 
		const std::vector<uint32>& indices, 
		const std::vector<texture>& textures)
	{
		m_vertices = vertices;
		m_indices = indices;
		m_textures = textures;

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);


		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Mesh::Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{ }

	void Mesh::Render(shader& shader)
	{
		uint32 diffuseN = 1, specularN = 1;
		for (uint32 i = 0; i < m_textures.size(); i++) 
		{
			std::string num, name = m_textures[i].type();

			if (name == "diffuse")
				num = std::to_string(diffuseN++);
			else if (name == "specular")
				num = std::to_string(specularN++);

			m_textures[i].bind(i); // Bind the texture to slot i
			// TODO: Should not need cast
			shader.set_uniform("material." + name + num, (int32)i); // Pass slot i to the shader material struct
		}

		// draw mesh
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}

}

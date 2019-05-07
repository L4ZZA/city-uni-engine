#include "pch.h"
#include "mesh.h"

#include "glad/glad.h"


engine::mesh::mesh(
	const std::vector<vertex>& vertices,
	const std::vector<uint32>& indices,
	const std::vector<texture>& textures)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;

	m_va.create();
	m_buffer.create();
	m_index_buffer.create();

	m_va.bind();

	// sending vertex data to gpu
	m_buffer.bind();
	auto s = vertices.size() * sizeof(mesh::vertex);
	m_buffer.add_data(&vertices[0], vertices.size() * sizeof(mesh::vertex));
	m_buffer.send_to_gpu();

	// sending index data to gpu
	m_index_buffer.bind();
	m_index_buffer.add_data(&indices[0], indices.size());
	m_index_buffer.send_to_gpu();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::vertex), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::vertex), reinterpret_cast<void*>(offsetof(vertex, normal)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mesh::vertex), reinterpret_cast<void*>(offsetof(vertex, tex_coords)));

	m_va.unbind();
}

engine::mesh::~mesh()
{}

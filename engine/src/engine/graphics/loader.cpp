#include "pch.h"
#include "loader.h"
#include "glad/glad.h"
#include "stb_image.h"

engine::raw_model engine::loader::load_model(
	const std::vector<float> &positions,
	const std::vector<float> &tex_coords,
	const std::vector<float> &normals,
	const std::vector<unsigned int> &indices)
{
	unsigned int vao = create_vao();
	store_data(0, 3, positions);
	store_data(1, 2, tex_coords);
	store_data(2, 3, normals);
	store_indices(indices);
	return raw_model(vao, indices.size());
}

unsigned int engine::loader::load_texture(const std::string &file_name)
{
	unsigned int m_id;
	int width;
	int height;
	int bpp;
	const int channels = 4; // RGBA

	// TODO: only flip if jpg
	stbi_set_flip_vertically_on_load(false);
	unsigned char* buffer = stbi_load(file_name.c_str(), &width, &height, &bpp, channels);

	glGenTextures(1, &m_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_textures.push_back(m_id);
	return m_id;
}

void engine::loader::cleanup()
{
	for(const auto vao : m_vaos)
		glDeleteVertexArrays(1, &vao);

	for(const auto vbo : m_vbos)
		glDeleteBuffers(1, &vbo);

	for(const auto tex : m_textures)
		glDeleteBuffers(1, &tex);
}

unsigned int engine::loader::create_vao()
{
	unsigned int vao_id;
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	m_vaos.push_back(vao_id);
	return vao_id;
}

void engine::loader::store_data(int attribute_number, int count, const std::vector<float> &data)
{
	unsigned int vbo_id;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(attribute_number);
	glVertexAttribPointer(attribute_number, count, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_vbos.push_back(vbo_id);
}

void engine::loader::store_indices(const std::vector<unsigned int> &indices)
{
	unsigned int vio_id;
	glGenBuffers(1, &vio_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_vbos.push_back(vio_id);
}

void engine::loader::bind_vao()
{}

void engine::loader::unbind_vao()
{
	glBindVertexArray(0);
}

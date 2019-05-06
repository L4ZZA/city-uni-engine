#include "pch.h"
#include "buffer.h"
#include "glad/glad.h"

engine::buffer::~buffer()
{
	glDeleteBuffers(1, &m_id);
	m_vertex_data.clear();
}

void engine::buffer::create()
{
	glGenBuffers(1, &m_id);
}

void engine::buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void engine::buffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void engine::buffer::add_data(const void* data, unsigned int size)
{
	m_vertex_data.insert(m_vertex_data.end(), (unsigned char*) data, (unsigned char*) data + size);
}

void engine::buffer::send_to_gpu()
{
	auto s = m_vertex_data.size();
	glBufferData(GL_ARRAY_BUFFER, m_vertex_data.size(), &m_vertex_data[0], GL_STATIC_DRAW);
	m_vertex_data.clear();
}

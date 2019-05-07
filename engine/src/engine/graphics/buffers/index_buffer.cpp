#include "pch.h"
#include "index_buffer.h"
#include "glad/glad.h"

engine::index_buffer::index_buffer()
{
	glGenBuffers(1, &m_id);
}

engine::index_buffer::~index_buffer()
{
	glGenBuffers(1, &m_id);
}

void engine::index_buffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void engine::index_buffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

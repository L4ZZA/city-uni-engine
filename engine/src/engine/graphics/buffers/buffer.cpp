#include "pch.h"
#include "buffer.h"
#include "glad/glad.h"

engine::buffer::buffer()
{
	glGenBuffers(1, &m_id);
}

engine::buffer::~buffer()
{
	glDeleteBuffers(1, &m_id);
}

void engine::buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void engine::buffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

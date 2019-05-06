#include "pch.h"
#include "vertex_array.h"
#include "glad/glad.h"

engine::vertex_array::~vertex_array()
{
	//glDeleteVertexArrays(1, &m_id);
}

void engine::vertex_array::create()
{
	glGenVertexArrays(1, &m_id);
}

void engine::vertex_array::bind() const
{
	glBindVertexArray(m_id);
}

void engine::vertex_array::unbind() const
{
	glBindVertexArray(0);
}

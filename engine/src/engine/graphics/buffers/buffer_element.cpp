#include "pch.h"
#include "buffer_element.h"
#include "glad/glad.h"

engine::buffer_element::buffer_element()
{
	glGenBuffers(1, &m_id);
}

engine::buffer_element::~buffer_element()
{
	glGenBuffers(1, &m_id);
}

void engine::buffer_element::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void engine::buffer_element::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

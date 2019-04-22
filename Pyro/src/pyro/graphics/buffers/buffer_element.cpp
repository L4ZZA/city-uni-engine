#include "pyro_pch.h"
#include "buffer_element.h"
#include "glad/glad.h"

pyro::buffer_element::buffer_element()
{
	glGenBuffers(1, &m_id);
}

pyro::buffer_element::~buffer_element()
{
	glGenBuffers(1, &m_id);
}

void pyro::buffer_element::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void pyro::buffer_element::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

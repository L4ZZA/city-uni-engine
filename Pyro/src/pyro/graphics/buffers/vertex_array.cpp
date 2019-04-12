#include "pyro_pch.h"
#include "vertex_array.h"
#include "glad/glad.h"

pyro::vertex_array::vertex_array()
{
    glGenVertexArrays(1, &m_id);
}

pyro::vertex_array::~vertex_array()
{
    glDeleteVertexArrays(1, &m_id);
}

void pyro::vertex_array::bind() const
{
    glBindVertexArray(m_id);
}

void pyro::vertex_array::unbind() const
{
    glBindVertexArray(0);
}

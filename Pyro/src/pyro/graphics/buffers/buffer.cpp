#include "pyro_pch.h"
#include "buffer.h"
#include "glad/glad.h"

pyro::buffer::buffer()
{
    glGenBuffers(1, &m_id);
}

pyro::buffer::~buffer()
{
    glDeleteBuffers(1, &m_id);
}

void pyro::buffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void pyro::buffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

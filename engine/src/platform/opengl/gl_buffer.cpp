#include "pch.h"
#include "gl_buffer.h"
#include <glad\glad.h>

// =========================================================================

static GLenum gl_usage(engine::e_vertex_buffer_usage usage)
{
	switch(usage)
	{
		case engine::e_vertex_buffer_usage::static_:    return GL_STATIC_DRAW;
		case engine::e_vertex_buffer_usage::dynamic:    return GL_DYNAMIC_DRAW;
	}
	CORE_ASSERT(false, "Unknown vertex buffer usage");
	return 0;
}

//=================== vertex buffer =======================================

engine::gl_vertex_buffer::gl_vertex_buffer(
    const void* data,
    uint32_t size,
    e_vertex_buffer_usage usage /*= e_vertex_buffer_usage::static_*/)
    :m_size(size), m_usage(usage)
{
    m_local_data = buffer::copy(data, size);
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, m_size, m_local_data.m_data, gl_usage(m_usage));
}

engine::gl_vertex_buffer::~gl_vertex_buffer()
{
    glDeleteBuffers(1, &m_id);
}

void engine::gl_vertex_buffer::set_data(
    void* data,
    uint32_t size,
    uint32_t offset /*= 0*/)
{
    m_local_data = buffer::copy(data, size);
    m_size = size;
    glNamedBufferSubData(m_id, offset, m_size, m_local_data.m_data);
}

void engine::gl_vertex_buffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void engine::gl_vertex_buffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void engine::gl_vertex_buffer::layout(const buffer_layout& layout)
{
    m_layout = layout;
}

const engine::buffer_layout& engine::gl_vertex_buffer::layout() const
{
    return m_layout;
}

//=================== index buffer ========================================

engine::gl_index_buffer::gl_index_buffer(const void* data, uint32_t size)
    : m_size(size)
{
    m_local_data = buffer::copy(data, size);
	glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
}

engine::gl_index_buffer::~gl_index_buffer()
{
    glDeleteBuffers(1, &m_id);
}

void engine::gl_index_buffer::set_data(void* data, uint32_t size, uint32_t offset)
{
    m_local_data = buffer::copy(data, size);
    m_size = size;
    glNamedBufferSubData(m_id, offset, m_size, m_local_data.m_data);
}

void engine::gl_index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void engine::gl_index_buffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#include "pch.h"
#include "index_buffer.h"
#include "glad/glad.h"

engine::index_buffer::index_buffer(const uint32* data, uint32 count)
{
	create();
	bind();
	add_data(data, count);
}

engine::index_buffer::~index_buffer()
{
	//unbind();
	//glDeleteBuffers(1, &m_id);
}

void engine::index_buffer::create()
{
	glGenBuffers(1, &m_id);
}

void engine::index_buffer::add_data(const uint32* data, uint32 count)
{
	m_data.insert(m_data.end(), (unsigned char*) data, (unsigned char*) data + count * sizeof(uint32));
	m_count += count;
}

void engine::index_buffer::send_to_gpu()
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data.size(), &m_data[0], GL_STATIC_DRAW);
	m_data.clear();
}

void engine::index_buffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void engine::index_buffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

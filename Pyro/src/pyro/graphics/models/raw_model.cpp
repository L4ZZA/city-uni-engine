#include "pyro_pch.h"
#include "raw_model.h"

pyro::raw_model::raw_model(unsigned int vao_id, size_t vertex_count)
{
    create(vao_id, vertex_count);
}

void pyro::raw_model::create(unsigned int vao_id, size_t vertex_count)
{
    m_vao_id = vao_id;
    m_vertex_count = static_cast<int>(vertex_count);
}

unsigned pyro::raw_model::vao_id() const
{
    return m_vao_id;
}

int pyro::raw_model::vertex_count() const
{
    return m_vertex_count;
}

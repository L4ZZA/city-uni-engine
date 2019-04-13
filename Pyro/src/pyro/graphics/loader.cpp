#include "pyro_pch.h"
#include "loader.h"
#include "glad/glad.h"

pyro::raw_model pyro::loader::load_model(const std::vector<float> &positions, const std::vector<unsigned int> &indices)
{
    unsigned int vao = create_vao();
    store_data(0, positions);
    store_indices(indices);
    return raw_model(vao, indices.size());
}

void pyro::loader::cleanup()
{
    for(const auto vao : m_vaos)
        glDeleteVertexArrays(1, &vao);

    for(const auto vbo : m_vbos)
        glDeleteBuffers(1, &vbo);
}

unsigned int pyro::loader::create_vao()
{
    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    m_vaos.push_back(vao_id);
    return vao_id;
}

void pyro::loader::store_data(int attribute_number, const std::vector<float> &data)
{
    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(attribute_number);
    glVertexAttribPointer(attribute_number, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_vbos.push_back(vbo_id);
}

void pyro::loader::store_indices(const std::vector<unsigned int> &indices)
{
    unsigned int vio_id;
    glGenBuffers(1, &vio_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_vbos.push_back(vio_id);
}

void pyro::loader::bind_vao()
{
}

void pyro::loader::unbind_vao()
{
    glBindVertexArray(0);
}

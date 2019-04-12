#include "pyro_pch.h"
#include "loader.h"
#include "glad/glad.h"

pyro::raw_model pyro::loader::load_model(const std::vector<float> &positions)
{
    unsigned int vao = create_vao();
    store_data_in_attributes_list(0, positions);
    return raw_model(vao, positions.size() / 3);
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

void pyro::loader::store_data_in_attributes_list(int attribute_number, const std::vector<float> &data)
{
    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, data.size() * 4, data.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(attribute_number);
    glVertexAttribPointer(attribute_number, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_vbos.push_back(vbo_id);
}

void pyro::loader::bind_vao()
{
}

void pyro::loader::unbind_vao()
{
    glBindVertexArray(0);
}

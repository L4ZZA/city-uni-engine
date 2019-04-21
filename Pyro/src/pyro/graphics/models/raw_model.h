#pragma once

namespace pyro
{
    class raw_model
    {
    public:
        raw_model() = default;
        raw_model(unsigned int vao_id, size_t vertex_count);
        void create(unsigned int vao_id, size_t vertex_count);

        unsigned int vao_id() const;
        int vertex_count() const;

    private:
        unsigned int    m_vao_id{0};
        int             m_vertex_count{0};
    };
}

#pragma once

namespace pyro
{
    class vertex_array
    {
    public:
        vertex_array();
        ~vertex_array();
        
        void bind() const;
        void unbind() const;

    private:
        unsigned int m_id;

    };
}

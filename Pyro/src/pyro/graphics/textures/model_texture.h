#pragma once

namespace pyro
{
    class model_texture
    {
    public:
        model_texture(unsigned int id)
            : m_id(id)
        {
        }

        unsigned int id() const { return m_id; }

    private:
        unsigned int m_id;
    };
}

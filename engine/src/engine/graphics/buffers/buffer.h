#pragma once

namespace engine
{
    class buffer
    {
    public:
        buffer();
        ~buffer();

        void bind() const;
        void unbind() const;

    private:
        unsigned int m_id;
    };
}

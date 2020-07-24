#pragma once

namespace engine
{
    struct buffer
    {
        byte*    m_data;
        uint32_t m_size;

        buffer()
            : m_data(nullptr), m_size(0)
        {
        }

        buffer(byte* data, uint32_t size)
            : m_data(data), m_size(size)
        {
        }

        static buffer copy(const void* data, uint32_t size)
        {
            buffer buffer;
            buffer.allocate(size);
            memcpy(buffer.m_data, data, size);
            return buffer;
        }

        void allocate(uint32_t size)
        {
            delete[] m_data;
            m_data = nullptr;

            if(size == 0)
                return;

            m_data = new byte[size];
            m_size = size;
        }

        void zero_initialize()
        {
            if(m_data)
                memset(m_data, 0, m_size);
        }

        void write(byte* data, uint32_t size, uint32_t offset = 0)
        {
            CORE_ASSERT(offset + size <= m_size, "Buffer overflow!");
            memcpy(m_data + offset, data, size);
        }

        operator bool() const
        {
            return m_data;
        }

        byte& operator[](int index)
        {
            return m_data[index];
        }

        byte operator[](int index) const
        {
            return m_data[index];
        }

        template<typename T>
        T* as()
        {
            return (T*)m_data;
        }

        inline uint32_t size() const { return m_size; }
    };
}

#pragma once
#include "engine/renderer/buffer.h"
#include "engine/core/buffer.h"

namespace engine
{
    //=================== vertex buffer =======================================

    /// \brief OpenGL specific implementation of the vertex_buffer interface
    class gl_vertex_buffer final : public vertex_buffer
    {
    public:
        gl_vertex_buffer(
            const void* data,
            uint32_t size,
            e_vertex_buffer_usage usage = e_vertex_buffer_usage::static_);
        ~gl_vertex_buffer();

        void set_data(void* data, uint32_t size, uint32_t offset = 0) override;
        void bind() const override;
        void unbind() const override;

        void layout(const buffer_layout& layout) override;
        const buffer_layout& layout() const override;
        uint32_t size() const override { return m_size; }

    private:
        uint32_t m_id{ 0 };
        uint32_t m_size{ 0 };
        e_vertex_buffer_usage m_usage;
        buffer_layout m_layout;
		buffer m_local_data;
    };

    //=================== index buffer ========================================

    /// \brief OpenGL specific implementation of the index_buffer interface
    class gl_index_buffer final : public index_buffer
    {
    public:
        gl_index_buffer(const void* data, uint32_t size);
        ~gl_index_buffer();

        void set_data(void* data, uint32_t size, uint32_t offset) override;
        void bind() const override;
        void unbind() const override;

        uint32_t count() const override { return m_size / sizeof(uint32_t);}
        uint32_t id() const override { return m_id; }
        uint32_t size() const override { return m_size; }

    private:
        uint32_t m_id{ 0 };
        uint32_t m_size{ 0 };
        buffer m_local_data;
    };
}

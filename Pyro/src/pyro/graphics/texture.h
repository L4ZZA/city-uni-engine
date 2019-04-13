#pragma once
#include "glad/glad.h"

namespace pyro
{
    enum class e_color_channels
    {
        grayscale = 1,
        rgb = 3,
        rgba = 4
    };

    // TODO: complete descriptions (http://docs.gl/gl4/glTexParameter)

    enum class e_texture_filters
    {
        // Returns the value of the texture element that is nearest (in Manhattan distance) to the specified texture coordinates.
        nearest = GL_NEAREST,
        // Returns the weighted average of the four texture elements that are closest to the specified texture coordinates.
        linear = GL_LINEAR,

        // -- min filter only --
        //
        nearest_nearest = GL_NEAREST_MIPMAP_NEAREST,
        //
        linear_nearest = GL_LINEAR_MIPMAP_NEAREST,
        //
        nearest_linear = GL_NEAREST_MIPMAP_LINEAR,
        //
        linear_linear = GL_LINEAR_MIPMAP_LINEAR,

    };

    enum class e_wrap
    {
        //
        clamp_to_edge = GL_CLAMP_TO_EDGE,
        //
        clamp_to_border = GL_CLAMP_TO_BORDER,
        //
        mirrored_repeat = GL_MIRRORED_REPEAT,
        //
        repeat = GL_REPEAT,
        //
        mirror_clamp_to_edge = GL_MIRROR_CLAMP_TO_EDGE,
    };

    class texture
    {
    public:
        texture() = default;
        texture(const std::string &path);

        void create(const std::string &path);
        void cleanup();

        void bind(unsigned int slot = 0) const;
        void unbind() const;

        void set_min_filter(const e_texture_filters& filter) const;
        void set_mag_filter(const e_texture_filters& filter) const;
        void set_wrap_s(const e_wrap& mode) const;
        void set_wrap_t(const e_wrap& mode) const;

        unsigned int id() const { return m_id; }

    private:
        std::string         m_path;
        unsigned char*      m_buffer{ nullptr };
        unsigned int        m_id{ 0 };
        int                 m_width{ 0 };
        int                 m_height{ 0 };
        int                 m_bpp{ 0 };
        e_color_channels    m_channels{ e_color_channels::rgba };
    };
}

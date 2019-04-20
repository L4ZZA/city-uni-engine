#include "pyro_pch.h"
#include "texture.h"
#include "glad/glad.h"
#include "stb_image.h"

pyro::texture::texture(const std::string &path)
{
    create(path);
}

void pyro::texture::create(const std::string &path)
{
    m_path = path;

    stbi_set_flip_vertically_on_load(true);
    m_buffer = stbi_load(path.c_str(),
        &m_width, &m_height, &m_bpp, static_cast<int>(m_channels));

    glGenTextures(1, &m_id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);
    unbind();

	if (m_buffer)
		stbi_image_free(m_buffer);
}

void pyro::texture::cleanup()
{
    SAFE_RELEASE(m_buffer);
}

void pyro::texture::bind(unsigned int slot /*= 0*/) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void pyro::texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void pyro::texture::set_min_filter(const e_texture_filters& filter) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(filter));
}

void pyro::texture::set_mag_filter(const e_texture_filters& filter) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(filter));
}

void pyro::texture::set_wrap_s(const e_wrap& mode) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(mode));
}

void pyro::texture::set_wrap_t(const e_wrap& mode) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(mode));
}

﻿#include "pch.h"
#include "texture.h"
#include "glad/glad.h"
#include "stb_image.h"
/// \brief Each enum value is the actual number of channels 
/// used by opengl for that format.
enum class engine::e_color_channels
{
	grayscale = 1,
	grayscale_alpha = 2,
	rgb = 3,
	rgba = 4,
};

/// \brief 
enum class engine::e_texture_filters
{
	none = 0,
	// Returns the value of the texture element that is nearest (in Manhattan distance) to the specified texture coordinates.
	nearest = GL_NEAREST,
	// Returns the weighted average of the four texture elements that are closest to the specified texture coordinates.
	linear = GL_LINEAR,
};

// TODO: complete descriptions (http://docs.gl/gl4/glTexParameter)

/// \brief 
enum class engine::e_wrap
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

engine::texture::texture(const std::string &path, const std::string &type /*= ""*/)
	: m_channels(e_color_channels::rgba)
{
	create(path, type);
}

void engine::texture::create(const std::string &path, const std::string &type /*= ""*/)
{
	m_path = path;
	m_type = type;

	stbi_set_flip_vertically_on_load(true);
	m_buffer = stbi_load(path.c_str(),
		&m_width, &m_height, &m_bpp, static_cast<int>(m_channels));

	glGenTextures(1, &m_id);
	bind();
	set_min_filter(e_texture_filters::linear);
	set_mag_filter(e_texture_filters::linear);
	set_wrap_s(e_wrap::clamp_to_edge);
	set_wrap_t(e_wrap::clamp_to_edge);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);
	unbind();

	if(m_buffer)
		stbi_image_free(m_buffer);
}

void engine::texture::cleanup()
{
	SAFE_RELEASE(m_buffer);
}

void engine::texture::bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void engine::texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void engine::texture::set_min_filter(const e_texture_filters& filter) const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(filter));
}

void engine::texture::set_mag_filter(const e_texture_filters& filter) const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(filter));
}

void engine::texture::set_wrap_s(const e_wrap& mode) const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(mode));
}

void engine::texture::set_wrap_t(const e_wrap& mode) const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(mode));
}

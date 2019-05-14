#pragma once
//#include "glad/glad.h"

namespace engine
{
	enum class e_color_channels;
	enum class e_texture_filters;
	enum class e_wrap;

	/// \brief Texture abstraction 
	class texture
	{
	public:
		texture() = default;
		texture(const std::string &path, const std::string &type = "");

		void create(const std::string &path, const std::string &type = "");
		void cleanup();

		void bind(unsigned int slot = 0) const;
		void unbind() const;

		void set_min_filter(const e_texture_filters& filter) const;
		void set_mag_filter(const e_texture_filters& filter) const;
		void set_wrap_s(const e_wrap& mode) const;
		void set_wrap_t(const e_wrap& mode) const;

		unsigned int id() const { return m_id; }
		std::string type() const { return m_type; }

	private:
		std::string         m_path;
		std::string         m_type;
		unsigned char*      m_buffer{nullptr};
		unsigned int        m_id{0};
		int                 m_width{0};
		int                 m_height{0};
		int                 m_bpp{0};
		e_color_channels    m_channels{0};
	};
}

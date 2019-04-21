#pragma once
#include "models/raw_model.h"

namespace pyro
{
	class loader
	{
	public:
		static raw_model load_model(
			const std::vector<float> &positions,
			const std::vector<float> &tex_coords,
			const std::vector<float> &normals,
			const std::vector<unsigned int> &indices);
		static unsigned int load_texture(const std::string &file_name);
		static void cleanup();

	private:
		static unsigned int create_vao();

		static void store_data(int attribute_number, int count, const std::vector<float> &data);
		static void store_indices(const std::vector<unsigned int> &indices);

		static void bind_vao();
		static void unbind_vao();

	private:
		inline static std::vector<unsigned int> m_vaos{};
		inline static std::vector<unsigned int> m_vbos{};
		inline static std::vector<unsigned int> m_textures{};
	};
}

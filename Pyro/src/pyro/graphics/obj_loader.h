#pragma once
#include "models/raw_model.h"

namespace pyro
{
	class obj_loader
	{
	public:

		template <typename T> static std::vector<T> parse(const std::vector<std::string> &components);
		template <typename T> static T parse(const std::string &component);

		/// \brief Goes through each triad of vertices that specifies it and 
		/// composes the textures and normals array so that the values are 
		/// in the right order.
		static void process_vertex(
			const std::vector<std::string> &components,
			std::vector<unsigned int> &indices,
			std::vector<glm::vec2> &tex_coords,
			std::vector<glm::vec3> &normals,
			std::vector<float> &textures_array,
			std::vector<float> &normals_array);

		static raw_model load_obj(const std::string &file_name);
	};

	// ======== templates implementation =======================================

	template <typename T>
	std::vector<T> obj_loader::parse(const std::vector<std::string>& components)
	{
			std::vector<T> comps;
			for(const auto& comp : components)
			{
				CORE_ASSERT(comp.empty(), "Should not be empty.");
				auto actual = atof(comp.c_str());
				T c = static_cast<T>(actual);
				comps.push_back(c);
			}
			return comps;
	}

	template <typename T>
	T obj_loader::parse(const std::string& component)
	{
			CORE_ASSERT(component.empty(), "Should not be empty.");
			auto actual = atof(component.c_str());
			return static_cast<T>(actual);
	}
}

#pragma once
#include "shapes/cuboid.h"

namespace engine
{
	class cuboid;

	/// \brief Class creating a terrain object containing a cuboid shape of a specified size
	class terrain : public cuboid
	{
	public:
		/// \brief Constructor
		terrain(glm::vec3 position, bool is_static, float length, float depth, float width, std::vector<ref<texture_2d>> textures);
		/// \brief Destructor
		~terrain();

		/// \brief Getter methods
		float width() { return m_width; }
		float length() { return m_length; }
		float depth() { return m_depth; }

	private:
		/// \brief Fields
		// terrain's dimensions
		float m_width;
		float m_depth;
		float m_length;
	};
}

#pragma once
#include "engine/entities/game_object.h"

namespace engine
{
	class texture_2d;

	/// \brief Class creating a cuboid object with a mesh of a specified size
	class cuboid : public game_object
	{
	public:
		/// \brief Constructor
		cuboid(bool is_static, glm::vec3 half_extents, const std::vector<ref<texture_2d>> textures, bool inwards);

		/// \brief Destructor
		~cuboid();

		/// \brief Getter methods
		glm::vec3 half_extents() { return m_half_extents; }
		bool inwards() { return m_inwards; }
	private:
		/// \brief Fields
		// Dimensions of the cuboid specified by a vector of half-extents. Cuboid is centered on the origin in local coordinates.
		glm::vec3 m_half_extents;

		// Boolean recording if the cube should be inverted (used in skybox)
		bool m_inwards;
	};
}

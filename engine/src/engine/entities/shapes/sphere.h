#pragma once
#include "engine/entities/game_object.h"

namespace engine
{
	class texture_2d;

	/// \brief Class creating a sphere object with a vertex array of a specified size and tesselation
	class sphere : public game_object
	{
	public:
		/// \brief Constructor
		sphere(glm::vec3 position, bool is_static, uint32_t stacks, uint32_t slices, float radius, const std::vector<ref<texture_2d>> textures);

		/// \brief Destructor
		~sphere();

		/// \brief Getter methods
		uint32_t stacks() { return m_stacks; }
		uint32_t slices() { return m_slices; }
		float radius() { return m_radius; }

	private:
		/// \brief Fields
		// Vertical (stacks) and horizontal (slices) tesselation level of the sphere
		uint32_t m_stacks;
		uint32_t m_slices;

		// Sphere's radius
		float m_radius;
	};
}

#pragma once
#include "engine/entities/game_object.h"

namespace engine
{
	class texture_2d;

	/// \brief Class creating a sphere object with a vertex array of a specified size and tesselation
	class sphere final : public game_object
	{
	public:
        // TODO - objects (any object for now, more edge cases will be explained on the way) should be passed as !!! const& !!! [one exception here is the smart pointer "ref"]
        // TODO - lines shouldn't exceed 100 characters, for better readability.
		/// \brief Constructor
		sphere(glm::vec3 position, bool is_static, uint32_t stacks, uint32_t slices, float radius, const std::vector<ref<texture_2d>> textures);

		/// \brief Destructor
		~sphere();

        // TODO - getters should always have a const specifier before the curly brackets since they don't modify any class member 
        /// \brief Getter methods
		uint32_t stacks() { return m_stacks; }
		uint32_t slices() { return m_slices; }
		float radius() { return m_radius; }

	private:
		/// \brief Fields
        // TODO - comments in header files should start with "/// \brief ", and there should be one per member/method so that intellisense can show tooltips properly
		// Vertical (stacks) and horizontal (slices) tessellation level of the sphere
		uint32_t m_stacks;
		uint32_t m_slices;

		// Sphere's radius
		float m_radius;
	};
}

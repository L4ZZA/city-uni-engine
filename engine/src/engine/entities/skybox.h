#pragma once
#include "game_object.h"

namespace engine
{

	/// \brief Class creating a skybox object containing an inverted cube shape of a specified size
	class skybox : public game_object
	{
	public:
		/// \brief Constructor
		skybox(float size, glm::vec3 camera_position, float rotation, const std::vector<ref<texture_2d>> textures);
		/// \brief Destructor
		~skybox();

		/// \brief Methods
		// getter methods
		float size() { return m_size; }

		// update method keeping the skbox centered on the camera and updating the rotation (if skybox is rotating)
		void update(glm::vec3 camera_position, float rotation);

	private:
		/// \brief Fields
		// size of the skybox 
		float m_size;
	};
}

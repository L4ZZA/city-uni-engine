#pragma once

namespace engine
{
	class mesh;
	class texture_2d;

	/// \brief Class creating a skybox object containing an inverted cube shape of a specified size
	class skybox
	{
	public:
		/// \brief Constructor
		skybox(float size, std::vector<ref<texture_2d>> textures);
		/// \brief Destructor
		~skybox();

		/// \brief Methods
		// getter methods
		float size() const { return m_size; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		std::vector<ref<texture_2d>> textures() const { return m_textures; }

		static ref<skybox> create(float size, std::vector<ref<texture_2d>> textures);

	private:
		/// \brief Fields
		// size of the skybox 
		float m_size;

		// skybox mesh
		ref<engine::mesh> m_mesh;

		// skybox textures
		std::vector<ref<texture_2d>> m_textures;
	};
}

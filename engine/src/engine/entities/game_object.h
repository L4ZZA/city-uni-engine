#pragma once
#include "../graphics/mesh.h"
#include "../graphics/shader.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace engine
{
	class game_object
	{
	public:
		game_object(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 velocity, mesh mesh, const float scale, const bool is_static);
		~game_object();

		void move(double dt, glm::vec3 direction);
		glm::vec3 position() const { return m_position; }
		glm::vec3 direction() const { return m_direction;  }
		glm::vec3 velocity() const { return m_velocity; }
		mesh get_mesh() const { return m_mesh; }
		float scale() const { return m_scale; }
		bool is_static() const { return m_static; }

		void set_position(glm::vec3 position) { m_position = position; }
		void set_direction(glm::vec3 direction) { m_direction = direction; }
		void set_velocity(glm::vec3 velocity) { m_velocity = velocity; }
		void set_mesh(mesh mesh) { m_mesh = mesh; }
		void set_scale(float scale) { m_scale = m_scale; }

	private:
		glm::vec3	m_position{ 0.f };
		glm::vec3	m_direction{ 0.f };
		glm::vec3	m_velocity{ 0.f };
		mesh		m_mesh;
		float		m_scale;
		bool		m_static;

		std::string m_directory;
		std::string m_path;

	};
}

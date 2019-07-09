#pragma once
#include "../graphics/mesh.h"
#include "../graphics/shader.h"

namespace engine
{
	class game_object
	{
	public:
		game_object(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 velocity, engine::mesh mesh, const glm::vec3 scale, const bool is_static, float mass, int32 type, glm::vec3 bounding_shape, glm::vec3 rotation_axis, float rotation_amount);
		game_object(engine::mesh mesh, int32 type, glm::vec3 bounding_shape);
		~game_object();

		void move(double dt, glm::vec3 direction);
		glm::vec3 position() const { return m_position; }
		glm::vec3 direction() const { return m_direction; }
		glm::vec3 velocity() const { return m_velocity; }
		engine::mesh get_mesh() const { return m_mesh; }
		glm::vec3 scale() const { return m_scale; }
		bool is_static() const { return m_static; }
		float mass() const { return m_mass; }
		int32 type() const { return m_type; }
		glm::vec3 bounding_shape() const { return m_bounding_shape; }
		glm::vec3 rotation_axis() const { return m_rotation_axis; }
		float rotation_amount() const { return m_rotation_amount; }

		void set_position(glm::vec3 position) { m_position = position; }
		void set_direction(glm::vec3 direction) { m_direction = direction; }
		void set_velocity(glm::vec3 velocity) { m_velocity = velocity; }
		void set_mesh(engine::mesh mesh) { m_mesh = mesh; }
		glm::vec3 set_scale(glm::vec3 scale) { m_scale = m_scale; }
		void set_mass(float mass) { m_mass = mass; }
		void set_type(int32 type) { m_type = type; }
		void set_bounding_shape(glm::vec3 bounding_shape) { m_bounding_shape = bounding_shape; }
		void set_rotation_axis(glm::vec3 rotation_axis) { m_rotation_axis = rotation_axis; }
		void set_rotation_amount(float rotation_amount) { m_rotation_amount = rotation_amount; }

	private:
		glm::vec3	m_position{ 0.f };
		glm::vec3	m_direction{ 0.f };
		glm::vec3	m_velocity{ 0.f };
		glm::vec3	m_rotation_axis{ 0.f };
		float		m_rotation_amount{ 0.f };
		engine::mesh		m_mesh;
		glm::vec3		m_scale;
		bool		m_static;
		float		m_mass;

		std::string m_directory;
		std::string m_path;

		int32 m_type;

		glm::vec3	m_bounding_shape{ 0.f };

	};
}

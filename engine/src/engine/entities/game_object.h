#pragma once


namespace engine
{
	class mesh;
	class texture_2d;
	class vertex_array;
	class shader;

	/// \brief Base class for storing the physical information about the objects in the game
	class game_object
    {
    public:
        // TODO - lines shouldn't exceed 100 characters, for better readability.
        // TODO - comments in header files should start with "/// \brief ", and there should be one per member/method so that intellisense can show tooltips properly
		game_object(const glm::vec3 position, const glm::vec3 velocity, std::vector<ref<mesh>> meshes, const glm::vec3 scale, const bool is_static, float mass, int32_t type, glm::vec3 bounding_shape, glm::vec3 rotation_axis, float rotation_amount);
		game_object(const bool is_static, int32_t type, glm::vec3 bounding_shape);
		game_object(std::vector<ref<mesh>> meshes, int32_t type, glm::vec3 bounding_shape);
        // TODO - [IMPORTANT] classes that will be inherited by other ones have to have their destructor virtual or they won't be destructed properly!! 
		/// \brief Destructor -TODO: comments like these are not useful. If the method does something other ppl need to be aware of that's what you should write.
		~game_object();

		/// \brief Getter methods
		glm::vec3 position() const { return m_position; }
		glm::vec3 velocity() const { return m_velocity; }
		glm::vec3 acceleration() const { return m_acceleration; }

		glm::vec3 right() const { return m_right; }
		glm::vec3 up() const { return m_up; }
		glm::vec3 forward() const { return m_forward; }

		glm::vec3 rotation_axis() const { return m_rotation_axis; }
		float rotation_amount() const { return m_rotation_amount; }
		glm::vec3 angular_velocity() const { return m_angular_velocity; }
		glm::vec3 torque() const { return m_torque; }

		glm::vec3 scale() const { return m_scale; }
		bool is_static() const { return m_static; }
		float mass() const { return m_mass; }
		int32_t type() const { return m_type; }
		glm::vec3 bounding_shape() const { return m_bounding_shape; }
		const std::vector<ref<mesh>>& meshes() const { return m_meshes; }
		const std::vector<ref<texture_2d>>& textures() const { return m_textures; }
		const ref<vertex_array>& va() const { return m_va; }

		glm::vec3 offset() { return m_offset; }

		/// \brief Setter methods
		void set_position(glm::vec3 position) { m_position = position; }
		void set_velocity(glm::vec3 velocity) { m_velocity = velocity; }
		void set_acceleration(glm::vec3 acceleration) { m_acceleration = acceleration; }

		void set_right(glm::vec3 right) { m_right = right; }
		void set_up(glm::vec3 up) { m_up = up; }
		void set_forward(glm::vec3 forward) { m_forward = forward; }

		void set_rotation_axis(glm::vec3 rotation_axis) { m_rotation_axis = rotation_axis; }
		void set_rotation_amount(float rotation_amount) { m_rotation_amount = rotation_amount; }
		void set_angular_velocity(glm::vec3 angular_velocity) { m_angular_velocity = angular_velocity; }
		void set_torque(glm::vec3 torque) { m_torque = torque; }

		void set_scale(glm::vec3 scale) { m_scale = scale; }
		void set_mass(float mass) { m_mass = mass; }
		void set_type(int32_t type) { m_type = type; }
		void set_bounding_shape(glm::vec3 bounding_shape) { m_bounding_shape = bounding_shape; }
		void set_mesh(ref<mesh> mesh) { m_meshes.push_back(mesh); }
		void set_mesh(std::vector<ref<mesh>> meshes) { m_meshes = meshes; }
		void set_textures(std::vector<ref<texture_2d>> textures) { m_textures = textures; }
		void set_va(ref<vertex_array> va) { m_va = va; }
		void set_offset(glm::vec3 offset) { m_offset = offset; }

		void set_rotation(glm::vec3 direction);

		const glm::mat4& transform();
		void bind_textures();

    private:
		// TODO - comments in header files should start with "/// \brief ", and there should be one per member/method so that intellisense can show tooltips properly
		/// \brief Fields
		// object's position, velocity and acceleration vectors
		glm::vec3		m_position{ 0.f };
		glm::vec3		m_velocity{ 0.f };
		glm::vec3		m_acceleration{ 0.f };

		// object's orientation vectors
		glm::vec3		m_right{ 1.0f, 0.0f, 0.0f };
		glm::vec3		m_up{ 0.0f, 1.0f, 0.0f };
		glm::vec3		m_forward{ 0.0f, 0.0f, 1.0f };

		// object's rotation axis vector and rotation amount variable
		glm::vec3		m_rotation_axis{ 0.0f, 1.0f, 0.0f };
		float			m_rotation_amount{ 0.f };

		// object's angular velocity and torque vectors
		glm::vec3		m_angular_velocity{ 0.0f };
		glm::vec3		m_torque{ 0.0f };

		// object's meshes
		std::vector<ref<mesh>> m_meshes;
		std::vector<ref<texture_2d>> m_textures;
		ref<vertex_array> m_va;

		// object's scale vector, mass variable and static switch
		glm::vec3		m_scale{ 1.0f };
		float			m_mass{ 1.0f };
        // TODO - static member variable should start with s_ prefix.
		bool			m_static{ false };

		// object's bounding shape type and dimensions
		int32_t			m_type;
		glm::vec3		m_bounding_shape{ 0.f };

		glm::vec3 m_offset = glm::vec3(0.0f);
    };
}

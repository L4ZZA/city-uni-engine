#include "pch.h"
#include "game_object.h"

engine::game_object::game_object(const glm::vec3 position, const glm::vec3 velocity, std::vector<ref<mesh>> meshes,
	const glm::vec3 scale, const bool is_static, float mass, int32_t type, glm::vec3 bounding_shape,
	glm::vec3 rotation_axis, float rotation_amount)
	: m_position(position), m_velocity(velocity), m_scale(scale), m_static(is_static), m_meshes(meshes), m_mass(mass),
	m_type(type), m_bounding_shape(bounding_shape), m_rotation_axis(rotation_axis), m_rotation_amount(rotation_amount)
{
}

engine::game_object::game_object(std::vector<ref<mesh>> meshes, int32_t type, glm::vec3 bounding_shape)
    : m_meshes(meshes), m_type(type), m_bounding_shape(bounding_shape)
{
}

engine::game_object::game_object(const glm::vec3 position, const bool is_static, int32_t type, glm::vec3 bounding_shape)
	: m_position(position), m_static(is_static), m_type(type), m_bounding_shape(bounding_shape)
{
}

engine::game_object::~game_object() {}

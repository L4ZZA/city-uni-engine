#include "pch.h"
#include "game_object.h"
#include "engine/renderer/texture.h"

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

engine::game_object::game_object(const bool is_static, int32_t type, glm::vec3 bounding_shape)
	: m_static(is_static), m_type(type), m_bounding_shape(bounding_shape)
{
}

engine::game_object::~game_object() {}

const glm::mat4& engine::game_object::transform()
{
	// apply object's transformations including an offset to synchronise it with the bullet_physics simulation
	// by centering the object's centre of mass on its position
	glm::mat4 object_transform = glm::mat4(1.0f);
	object_transform = glm::translate(object_transform, position() - offset());
	object_transform = glm::scale(object_transform, scale());
	object_transform = glm::rotate(object_transform, rotation_amount(), rotation_axis());

	return object_transform;
}

void engine::game_object::bind_textures()
{
	// if the object has any textures stored, bind them
	if (m_textures.size() > 0)
	{
		auto object_textures = textures();
		for (const auto& texture : object_textures)
		{
			texture->bind();
		}
	}
}

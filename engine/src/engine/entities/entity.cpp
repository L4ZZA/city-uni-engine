#include "pch.h"
#include "entity.h"

engine::entity::entity(
	const textured_model &model,
	const glm::vec3 &pos /*= {0,0,0}*/,
	const glm::vec3 &rotation /*= {0,0,0}*/,
	float scale /*= 1*/)
{
	create(model, pos, rotation, scale);
}

void engine::entity::create(
	const textured_model &model,
	const glm::vec3 &pos /*= {0,0,0}*/,
	const glm::vec3 &rotation /*= {0,0,0}*/,
	float scale /*= 1*/)
{
	m_model = model;
	m_position = pos;
	m_rotation = rotation;
	m_scale = scale;
}

void engine::entity::increase_translation(const glm::vec3 &vec)
{
	m_position += vec;
}

void engine::entity::increase_rotation(const glm::vec3 &vec)
{
	m_rotation += vec;
}

engine::textured_model engine::entity::model() const
{
	return m_model;
}

glm::vec3 engine::entity::position() const
{
	return m_position;
}

glm::vec3 engine::entity::rotation() const
{
	return m_rotation;
}

float engine::entity::scale() const
{
	return m_scale;
}

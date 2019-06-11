#include "pch.h"
#include "game_object.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

game_object::game_object(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 velocity, engine::mesh mesh, const float scale, const bool is_static, float mass, int type, glm::vec3 bounding_shape)
	:m_position(position), m_direction(direction), m_velocity(velocity), m_scale(scale), m_static(is_static), m_mesh(mesh), m_mass(mass), m_type(type), m_bounding_shape(bounding_shape)
{}

game_object::~game_object() {}

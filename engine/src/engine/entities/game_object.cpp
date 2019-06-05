#include "pch.h"
#include "game_object.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

engine::game_object::game_object(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 velocity, mesh mesh, const float scale, const bool is_static, const std::string& path)
	:m_path(path), m_position(position), m_direction(direction), m_velocity(velocity), m_scale(scale), m_static(is_static), m_mesh(mesh)
{ }

engine::game_object::~game_object() {}

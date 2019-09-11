#include "pch.h"
#include "terrain.h"

engine::terrain::terrain(glm::vec3 position, bool is_static, float length, float depth, float width, std::vector<ref<texture_2d>> textures) : cuboid(position, is_static, glm::vec3(length, depth, width), textures, false), m_length(length), m_depth(depth), m_width(width) {}

engine::terrain::~terrain() {}

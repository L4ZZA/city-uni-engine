#include "pch.h"
#include "game_object.h"

engine::game_object::game_object(
    const glm::vec3 position,
    const glm::vec3 direction,
    const glm::vec3 velocity,
    /*engine::mesh mesh,*/
    const glm::vec3 scale,
    const bool is_static,
    float mass, int32_t type,
    glm::vec3 bounding_shape,
    glm::vec3 rotation_axis,
    float rotation_amount)
    :m_position(position),
    m_direction(direction),
    m_velocity(velocity),
    m_scale(scale),
    m_static(is_static),
    //m_mesh(mesh),
    m_mass(mass),
    m_type(type),
    m_bounding_shape(bounding_shape),
    m_rotation_axis(rotation_axis),
    m_rotation_amount(rotation_amount)
{
}

engine::game_object::game_object(/*engine::mesh mesh,*/ int32_t type, glm::vec3 bounding_shape)
    : m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_direction(glm::vec3(1.0f, 0.0f, 0.0f)),
    m_velocity(0.0f),
    m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
    m_static(false),
    //m_mesh(mesh), 
    m_mass(1.0f),
    m_type(type),
    m_bounding_shape(bounding_shape),
    m_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_rotation_amount(1.0f)
{
}

engine::game_object::~game_object() {}

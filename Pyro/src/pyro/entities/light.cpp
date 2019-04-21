#include "pyro_pch.h"
#include "light.h"

void pyro::light::create(const glm::vec3 &pos, const glm::vec3 &colour)
{
	m_position = pos;
	m_colour = colour;
}

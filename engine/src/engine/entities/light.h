#pragma once

namespace engine
{
	class light
	{
	public:
		void create(const glm::vec3 &pos, const glm::vec3 &colour);

		glm::vec3 position() const { return m_position; }
		glm::vec3 colour() const { return m_colour; }

	private:
		glm::vec3 m_position{ 0 };
		glm::vec3 m_colour{ 0 };
	};
}

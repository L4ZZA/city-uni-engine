﻿#pragma once

namespace engine
{
	class camera
	{
	public:
		void move(double dt);
		glm::vec3 position() const { return m_position; }
		float pitch() const { return m_pitch; }
		float yaw()   const { return m_yaw; }
		float roll()  const { return m_roll; }

	private:
		glm::vec3	m_position{ 0.f };
		/// \brief Rotation over strafe axis (how high or low the camera is aiming)
		float		m_pitch{ 0.f };
		/// \brief Rotation over the up axis (aiming left and right)
		float		m_yaw{ 0.f };
		/// \brief Rotation front axis (tilting left and right)
		float		m_roll{ 0.f };

		inline static float s_speed{ 5.0f };
	};
}
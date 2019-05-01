#pragma once

namespace engine
{
	/// \brief Base class for input polling.
	/// Input polling allows to retrieve input states at any time.
	class input
	{
	public:
		/// \brief Returns if the specified key is being pressed.
		static bool key_pressed(int key_code) { return s_instance->key_pressed_impl(key_code); }

		/// \brief Returns if the specified mouse button is being pressed.
		static bool mouse_button_pressed(int button) { return s_instance->mouse_button_pressed_impl(button); }

		/// \brief 
		static std::pair<float, float> mouse_position() { return s_instance->mouse_position_impl(); }
		/// \brief
		static float mouse_x() { return s_instance->mouse_x_impl(); }
		/// \brief
		static float mouse_y() { return s_instance->mouse_y_impl(); }

	protected:
		virtual bool key_pressed_impl(int key_code) const = 0;
		virtual bool mouse_button_pressed_impl(int key_code) const = 0;
		virtual std::pair<float, float> mouse_position_impl() const = 0;
		virtual float mouse_x_impl() const = 0;
		virtual float mouse_y_impl() const = 0;

		static input* s_instance;
	};
}
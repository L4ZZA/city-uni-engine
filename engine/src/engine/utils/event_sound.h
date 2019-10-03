#pragma once
#include "sound.h"

// TODO: move all audio relate stuff in engine/audio/ folder

namespace engine
{
	/// \brief Short lasting sound, generally paired up with events
	class event_sound : public sound
	{

	public:
		event_sound() = default;
		event_sound(const std::string &name);
		~event_sound() = default;

		bool load(const std::string &file_path, bool loop = false);
		bool play() override;

	};
}



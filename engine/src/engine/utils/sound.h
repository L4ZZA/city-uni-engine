#pragma once
#include "engine.h"
#include <fmod.hpp>

// TODO: move all audio relate stuff in engine/audio/ folder

namespace engine
{
	class sound
	{
	protected:
		std::string         m_name{};
		FMOD::Sound *       m_sound{ nullptr };
		FMOD::Channel*      m_channel{ nullptr };
	public:
		virtual ~sound()
		{
			SAFE_RELEASE(m_sound);
			SAFE_RELEASE(m_channel);
		}
		virtual bool play() = 0;
	};
}


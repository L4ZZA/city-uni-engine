#pragma once

namespace engine
{
	class core
	{
	public:
		static void startup();
		static void shutdown();

		static void platform_startup();
		static void platform_shutdown();

		static bool startup_success;

	private:
		core();
		virtual ~core();
	};

}

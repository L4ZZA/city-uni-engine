#pragma once

namespace pyro
{

	class core
	{
	public:
		static void startup();
		static void shutdown();

		static void platform_startup();
		static void platform_shutdown();

		static bool StartupSuccess;

	private:
		core();
		virtual ~core();
	};

}

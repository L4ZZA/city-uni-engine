#pragma once
#include "engine/defs.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace engine
{
	/// \brief Wrapper around spdlog logger.
	class ENGINE_API logger
	{
	public:
		static void init(
			spdlog::level::level_enum coreLevel = spdlog::level::trace,
			spdlog::level::level_enum clientLevel = spdlog::level::trace);
		static std::shared_ptr<spdlog::logger>& core_logger() { return s_core_logger; }
		static std::shared_ptr<spdlog::logger>& client_logger() { return s_client_logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_core_logger;
		static std::shared_ptr<spdlog::logger> s_client_logger;
	};
}

#ifndef ENGINE_DIST

// Core log macros
#define ENGINE_CORE_TRACE(...)    ::engine::logger::core_logger()->trace(__VA_ARGS__)
#define ENGINE_CORE_INFO(...)     ::engine::logger::core_logger()->info(__VA_ARGS__)
#define ENGINE_CORE_WARN(...)     ::engine::logger::core_logger()->warn(__VA_ARGS__)
#define ENGINE_CORE_ERROR(...)    ::engine::logger::core_logger()->error(__VA_ARGS__)
#define ENGINE_CORE_FATAL(...)    ::engine::logger::core_logger()->critical(__VA_ARGS__)

 // Client log macros
#define ENGINE_TRACE(...)	      ::engine::logger::client_logger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...)	      ::engine::logger::client_logger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)	      ::engine::logger::client_logger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)	      ::engine::logger::client_logger()->error(__VA_ARGS__)
#define ENGINE_FATAL(...)	      ::engine::logger::client_logger()->critical(__VA_ARGS__)

  // Render Queue logger
#ifdef LOG_RQ
#define ENGINE_RQ_FATAL(...)   ::engine::logger::core_logger()->critical(__VA_ARGS__)
#define ENGINE_RQ_ERROR(...)   ::engine::logger::core_logger()->error(__VA_ARGS__)
#define ENGINE_RQ_WARN(...)    ::engine::logger::core_logger()->warn(__VA_ARGS__)
#define ENGINE_RQ_INFO(...)    ::engine::logger::core_logger()->info(__VA_ARGS__)
#define ENGINE_RQ_TRACE(...)   ::engine::logger::core_logger()->trace(__VA_ARGS__)
#else
#define ENGINE_RQ_FATAL(...)
#define ENGINE_RQ_ERROR(...)
#define ENGINE_RQ_WARN(...)
#define ENGINE_RQ_INFO(...)
#define ENGINE_RQ_TRACE(...)
#endif

#endif

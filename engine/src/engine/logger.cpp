﻿#include "pch.h"
#include "logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> engine::logger::s_core_logger;
std::shared_ptr<spdlog::logger> engine::logger::s_client_logger;

void engine::logger::init(
	spdlog::level::level_enum coreLevel /*= spdlog::level::trace*/,
	spdlog::level::level_enum clientLevel /*= spdlog::level::trace*/)
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_core_logger = spdlog::stdout_color_mt("PYRO");
	s_core_logger->set_level(spdlog::level::trace);
	s_client_logger = spdlog::stdout_color_mt("APP");
	s_client_logger->set_level(spdlog::level::trace);
}
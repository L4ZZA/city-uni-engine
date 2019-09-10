#include "pch.h"
#include "render_command.h"

#include "platform/opengl/gl_renderer_api.h"

engine::renderer_api* engine::render_command::s_renderer_api = new engine::gl_renderer_api;

#pragma once

#include "render_command.hpp"
#include "pyro/graphics/render_manager.h"

#include "pyro/defs.h"
#include "pyro/logger.h"

#include "glad/glad.h"

#include <string>

namespace pyro::graphics::Command
{
  // --------------------------------------------------------------------------
  // -- SetClearColor                                                        --
  // --------------------------------------------------------------------------
  class SetClearColor : Command::RenderCommand {
  public:
    SetClearColor(float r, float g, float b)
      :r(r), g(g), b(b)
    {}

    void Execute() override {
      glClearColor(r, g, b, 1.0f);
      PYRO_RQ_TRACE("[RenderQueue] -- Command::SetClearColor: <{0}, {1}, {2}>", r, g, b);
    }

  private:
    float r;
    float g;
    float b;

  public: //STATIC CREATOR
    static void Dispatch(float r, float g, float b) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(SetClearColor));
      new (mem) SetClearColor(r, g, b);
    }
  };

  // --------------------------------------------------------------------------
  // -- ClearBuffer                                                          --
  // --------------------------------------------------------------------------
  class ClearBuffer : Command::RenderCommand {
  public:
    ClearBuffer()
    {}

    void Execute() override {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      PYRO_RQ_TRACE("[RenderQueue] -- Command::ClearBuffer");
    }

  public: //STATIC CREATOR
    static void Dispatch() {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(ClearBuffer));
      new (mem) ClearBuffer();
    }
  };

}

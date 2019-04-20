#pragma once

#include "renderCommand.hpp"
#include "pyro/graphics/renderManager.h"

#include "pyro/defs.h"
#include "pyro/log.h"
#include "pyro/application.h"

#include <string>

namespace pyro::graphics::Command
{
  // --------------------------------------------------------------------------
  // -- RenderImGUI                                                          --
  // --------------------------------------------------------------------------
  class RenderImGUI : Graphics::Command::RenderCommand
  {
  public:
    RenderImGUI(Application* self)
      :self(self)
    {}

    void Execute() override {
      // Render ImGui interface
      self->imguiLayer->Begin();

      for (Layer* layer : self->layerStack)
        layer->OnImGuiRender();

      self->imguiLayer->End();

      PYRO_RQ_TRACE("[RenderQueue] -- Command::RenderImGUI");
    }

  private:
    Application* self;

  public: //STATIC CREATOR
    static void Dispatch(Application* self) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(RenderImGUI));
      new (mem) RenderImGUI(self);
    }
  };

}

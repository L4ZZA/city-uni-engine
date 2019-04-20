#pragma once

#include "renderCommand.hpp"
#include "pyro/graphics/renderManager.h"

#include "pyro/defs.h"
#include "pyro/log.h"

#include "glad/glad.h"

#include <string>

namespace pyro::graphics::Command
{
  // --------------------------------------------------------------------------
  // -- UnloadTexture                                                        --
  // --------------------------------------------------------------------------
  class UnloadTexture : Command::RenderCommand {
  public:
    UnloadTexture(uint32 textureId)
      :textureId(textureId)
    {}

    void Execute() override {
      glDeleteTextures(1, &textureId);

      PYRO_RQ_TRACE("[RenderQueue] -- Command::UnloadTexture (texture {0})", textureId);
    }

  private:
    uint32 textureId;

  public: //STATIC CREATOR
    static void Dispatch(uint32 textureId) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(UnloadTexture));
      new (mem) UnloadTexture(textureId);
    }
  };

  // --------------------------------------------------------------------------
  // -- BindTexture                                                        --
  // --------------------------------------------------------------------------
  class BindTexture : Command::RenderCommand {
  public:
    BindTexture(uint32 textureId, uint32 slot)
      :textureId(textureId), slot(slot)
    {}

    void Execute() override {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, textureId);

      PYRO_RQ_TRACE("[RenderQueue] -- Command::BindTexture (texture {0}): slot {1}", textureId, slot);
    }

  private:
    uint32 textureId;
    uint32 slot;

  public: //STATIC CREATOR
    static void Dispatch(uint32 textureId, uint32 slot) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(BindTexture));
      new (mem) BindTexture(textureId, slot);
    }
  };

}

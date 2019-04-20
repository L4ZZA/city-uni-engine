#pragma once

#include "render_command.hpp"
#include "pyro/graphics/render_manager.h"

#include "pyro/defs.h"
#include "pyro/logger.h"

#include "glad/glad.h"

namespace pyro
{
  // --------------------------------------------------------------------------
  // -- UnloadTexture                                                        --
  // --------------------------------------------------------------------------
  class UnloadTexture : render_command
{
  public:
    UnloadTexture(uint32 textureId)
      :textureId(textureId)
    {}

    void execute() override 
  	{
      glDeleteTextures(1, &textureId);

      PYRO_RQ_TRACE("[RenderQueue] -- UnloadTexture (texture {0})", textureId);
    }

  private:
    uint32 textureId;

  public: //STATIC CREATOR
    static void dispatch(uint32 textureId) 
  	{
      auto mem = render_manager::get()->send_command(sizeof(UnloadTexture));
      new (mem) UnloadTexture(textureId);
    }
  };

  // --------------------------------------------------------------------------
  // -- BindTexture                                                        --
  // --------------------------------------------------------------------------
  class BindTexture : render_command
{
  public:
    BindTexture(uint32 textureId, uint32 slot)
      :textureId(textureId), slot(slot)
    {}

    void execute() override 
  	{
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, textureId);

      PYRO_RQ_TRACE("[RenderQueue] -- BindTexture (texture {0}): slot {1}", textureId, slot);
    }

  private:
    uint32 textureId;
    uint32 slot;

  public: //STATIC CREATOR
    static void dispatch(uint32 textureId, uint32 slot) 
  	{
      auto mem = render_manager::get()->send_command(sizeof(BindTexture));
      new (mem) BindTexture(textureId, slot);
    }
  };

}

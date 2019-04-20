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
  // -- RenderMesh                                                           --
  // --------------------------------------------------------------------------
  class RenderMesh : Command::RenderCommand {
  public:
    RenderMesh(uint32 vao, uint32 num)
      :vao(vao), num(num)
    {}

    void Execute() override {
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      PYRO_RQ_TRACE("[RenderQueue] -- Command::RenderMesh (vao {0}): {1} faces", vao, num);
    }

  private:
    uint32 vao;
    uint32 num;

  public: //STATIC CREATOR
    static void Dispatch(uint32 vao, uint32 num) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(RenderMesh));
      new (mem) RenderMesh(vao, num);
    }
  };

}

#pragma once

namespace pyro::graphics::Command {

  class RenderCommand
  {
  public:
    virtual ~RenderCommand() {}

    virtual void Execute() = 0;
  };

}

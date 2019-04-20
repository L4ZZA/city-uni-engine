#pragma once

namespace pyro
{

  class render_command
  {
  public:
    virtual ~render_command() {}

    virtual void execute() = 0;
  };

}

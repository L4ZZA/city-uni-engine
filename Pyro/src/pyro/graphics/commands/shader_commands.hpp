#pragma once

#include "pyro/defs.h"
#include "pyro/logger.h"
#include "pyro/graphics/render_manager.h"
#include "render_command.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>

namespace pyro::graphics::Command
{
  // --------------------------------------------------------------------------
  // -- SetUniformMatrix4f                                                   --
  // --------------------------------------------------------------------------
  class SetUniformMatrix4f : RenderCommand {
  public:
    SetUniformMatrix4f(uint32 programId, const std::string& name, const glm::mat4& mat)
      :programId(programId), name(name), mat(mat)
    {}

    void Execute() override {
      // TODO: change this to set uniform without binding
      glUseProgram(programId);
      int32 uniformLocation = glGetUniformLocation(programId, name.c_str());
      glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));

      PYRO_RQ_TRACE("[RenderQueue] -- Command::SetUniformMatrix4f (prog {0}): uniform: '{1}' = {2}(mat)", programId, name, mat);
    }

  private:
    uint32 programId;
    std::string name;
    glm::mat4 mat;

  public: //STATIC CREATOR
    static void Dispatch(uint32 programId, const std::string& name, const glm::mat4& mat) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(SetUniformMatrix4f));
      new (mem) SetUniformMatrix4f(programId, name, mat);
    }
  };

  // --------------------------------------------------------------------------
  // -- SetUniform1i                                                         --
  // --------------------------------------------------------------------------
  class SetUniform1i : RenderCommand {
  public:
    SetUniform1i(uint32 programId, const std::string& name, int32 val)
      :programId(programId), name(name), val(val)
    {}

    void Execute() override {
      // TODO: change this to set uniform without binding
      glUseProgram(programId);
      int32 uniformLocation = glGetUniformLocation(programId, name.c_str());
      glUniform1i(uniformLocation, val);

      PYRO_RQ_TRACE("[RenderQueue] -- Command::SetUniform1i (prog {0}): uniform: '{1}' = {2}(int32)", programId, name, val);
    }

  private:
    uint32 programId;
    std::string name;
    int32 val;

  public: // STATIC CREATOR
    static void Dispatch(uint32 programId, const std::string& name, int32 val) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(SetUniform1i));
      new (mem) SetUniform1i(programId, name, val);
    }
  };

  // --------------------------------------------------------------------------
  // -- SetUniform4f                                                         --
  // --------------------------------------------------------------------------
  class SetUniform4f : RenderCommand {
  public:
    SetUniform4f(uint32 programId, const std::string& name, const glm::vec4& vec)
      :programId(programId), name(name), vec(vec)
    {}

    void Execute() override {
      // TODO: change this to set uniform without binding
      glUseProgram(programId);
      int32 uniformLocation = glGetUniformLocation(programId, name.c_str());
      glUniform4f(uniformLocation, vec[0], vec[1], vec[2], vec[3]);

      PYRO_RQ_TRACE("[RenderQueue] -- Command::SetUniform4f (prog {0}): uniform: '{1}' = {2}(vec4)", programId, name, vec);
    }

  private:
    uint32 programId;
    std::string name;
    glm::vec4 vec;

  public: // STATIC CREATOR
    static void Dispatch(uint32 programId, const std::string& name, const glm::vec4& vec) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(SetUniform4f));
      new (mem) SetUniform4f(programId, name, vec);
    }
  };

  // --------------------------------------------------------------------------
  // -- SetUniform3f                                                         --
  // --------------------------------------------------------------------------
  class SetUniform3f : RenderCommand {
  public:
    SetUniform3f(uint32 programId, const std::string& name, const glm::vec3& vec)
      :programId(programId), name(name), vec(vec)
    {}

    void Execute() override {
      // TODO: change this to set uniform without binding
      glUseProgram(programId);
      int32 uniformLocation = glGetUniformLocation(programId, name.c_str());
      glUniform3f(uniformLocation, vec[0], vec[1], vec[2]);

      PYRO_RQ_TRACE("[RenderQueue] -- Command::SetUniform3f (prog {0}): uniform: '{1}' = {2}(vec3)", programId, name, vec);
    }

  private:
    uint32 programId;
    std::string name;
    glm::vec3 vec;

  public: // STATIC CREATOR
    static void Dispatch(uint32 programId, const std::string& name, const glm::vec3& vec) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(SetUniform3f));
      new (mem) SetUniform3f(programId, name, vec);
    }

  };

  // --------------------------------------------------------------------------
  // -- SetUniform1f                                                         --
  // --------------------------------------------------------------------------
  class SetUniform1f : RenderCommand {
  public:
    SetUniform1f(uint32 programId, const std::string& name, float val)
      :programId(programId), name(name), val(val)
    {}

    void Execute() override {
      // TODO: change this to set uniform without binding
      glUseProgram(programId);
      int32 uniformLocation = glGetUniformLocation(programId, name.c_str());
      glUniform1f(uniformLocation, val);

      PYRO_RQ_TRACE("[RenderQueue] -- Command::SetUniform1f (prog {0}): uniform: '{1}' = {2}(float)", programId, name, val);
    }

  private:
    uint32 programId;
    std::string name;
    float val;

  public: // STATIC CREATOR
    static void Dispatch(uint32 programId, const std::string& name, float val) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(SetUniform1f));
      new (mem) SetUniform1f(programId, name, val);
    }
  };

  // --------------------------------------------------------------------------
  // -- SetUniform1f                                                         --
  // --------------------------------------------------------------------------
  class BindShader : RenderCommand {
  public:
    BindShader(uint32 programId)
      :programId(programId)
    {}

    void Execute() override {
      // TODO: change this to set uniform without binding
      glUseProgram(programId);

      PYRO_RQ_TRACE("[RenderQueue] -- Command::BindShader (prog {0})", programId);
    }

  private:
    uint32 programId;

  public: // STATIC CREATOR
    static void Dispatch(uint32 programId) {
      auto mem = ::pyro::graphics::RenderManager::Get()->SubmitToQueue(sizeof(BindShader));
      new (mem) BindShader(programId);
    }
  };

}

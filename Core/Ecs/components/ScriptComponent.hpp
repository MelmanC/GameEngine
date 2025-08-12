#pragma once

#include <string>

namespace ecs {

  enum class ScriptState {
    NOT_LOADED,
    LOADED,
    ERROR,
    RUNNING
  };

  struct ScriptComponent {
      std::string scriptPath = "";

      ScriptState state = ScriptState::NOT_LOADED;
      std::string lastError;

      bool hasStarted = false;

      int luaRef = -1;
      bool enabled = false;
  };

}  // namespace ecs

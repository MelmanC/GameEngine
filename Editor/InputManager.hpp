#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>

namespace app {
  class Application;
}

namespace input {

  class InputManager {
    public:
      InputManager(app::Application& app);

      ~InputManager() = default;

      void handleInput();

    private:
      void handleKeyboardInput();
      void handleMouseInput();
      void handleCameraMovement();
      void handleApplicationShortcuts();
      void handleObjectSelection();

      bool canProcessKeyboard() const;
      bool canProcessMouse() const;

      app::Application& _app;
  };
}  // namespace input

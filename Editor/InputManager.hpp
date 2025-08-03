#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>

namespace app {
  class Application;
}

namespace ecs {
  class ECSManager;
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
      void handleEntitySelection();

      bool canProcessKeyboard() const;
      bool canProcessMouse() const;

      bool isMouseInViewport(const raylib::Vector2& mousePos) const;

      app::Application& _app;
  };
}  // namespace input

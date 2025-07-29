#pragma once

namespace gui {
  class Window;
}

namespace input {

  class InputManager {
    public:
      InputManager(gui::Window &window) : _window(window) {
      }
      ~InputManager() = default;
      void handleInput();

    private:
      gui::Window &_window;
  };

}  // namespace input

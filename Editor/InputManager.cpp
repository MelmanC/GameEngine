#include "InputManager.hpp"
#include "Window.hpp"

void input::InputManager::handleInput() {
  if (!ImGui::GetIO().WantCaptureKeyboard) {
    if (IsKeyPressed(KEY_ESCAPE)) {
      _window.setRunning(false);
    }

    if (IsKeyPressed(KEY_F)) {
      _window.setViewportActive(!_window.isViewportActive());
      if (_window.isViewportActive()) {
        auto _viewport = _window.getViewport();
        SetMousePosition(_viewport.x + _viewport.width / 2,
                         _viewport.y + _viewport.height / 2);
        DisableCursor();
      } else {
        EnableCursor();
      }
    }
  }
}

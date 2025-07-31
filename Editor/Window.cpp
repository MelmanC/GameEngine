#include "Window.hpp"

gui::Window::Window(int width, int height, const char* title)
    : _application(width, height, title) {
}

gui::Window::~Window() {
  _application.shutdown();
}

void gui::Window::run() {
  _application.run();
}

bool gui::Window::shouldClose() const {
  return _application.isRunning() == false;
}

void gui::Window::close() {
  _application.shutdown();
}

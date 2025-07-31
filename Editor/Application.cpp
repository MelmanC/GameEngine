#include "Application.hpp"
#include <imgui.h>
#include <rlImGui.h>
#include <iostream>

app::Application::Application(int width, int height, const char* title)
    : _width(width),
      _height(height),
      _title(title),
      _window(),
      _isRunning(true),
      _scene(),
      _camera(),
      _renderer(width, height),
      _viewportManager(width, height) {
  _window.Init(_width, _height, _title);
  _window.SetTargetFPS(60);

  rlImGuiSetup(true);

  _gui = std::make_unique<ui::Gui>(_width, _height);
  _inputManager = std::make_unique<input::InputManager>(*this);

  try {
    _scene.load("./scene.json");
  } catch (const std::exception& e) {
    std::cout << "Warning: Could not load scene.json: " << e.what()
              << std::endl;
  }
}

app::Application::~Application() {
  rlImGuiShutdown();
  if (_isRunning) {
    _window.Close();
  }
}

void app::Application::run() {
  while (_isRunning && !_window.ShouldClose()) {
    update();
    render();
  }
}

void app::Application::shutdown() {
  _isRunning = false;
}

void app::Application::update() {
  _viewportManager.updateCameraControls(_camera);

  _inputManager->handleInput();
}

void app::Application::render() {
  _renderer.beginFrame();

  _renderer.drawBackground(raylib::Color(45, 45, 48));

  _renderer.drawViewport(_viewportManager.getViewport(), _camera, _scene);

  _renderer.drawViewportFrame(_viewportManager.getViewport(),
                              _viewportManager.isActive());

  rlImGuiBegin();
  _gui->drawInterface(_camera, *this);
  rlImGuiEnd();

  _renderer.endFrame();
}

raylib::Rectangle app::Application::getViewport() const {
  return _viewportManager.getViewport();
}

bool app::Application::isViewportActive() const {
  return _viewportManager.isActive();
}

void app::Application::setViewportActive(bool active) {
  _viewportManager.setActive(active);
}

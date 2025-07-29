#include "Window.hpp"
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>
#include <raylib.hpp>

gui::Window::Window(int width, int height, const char *title)
    : _width(width),
      _height(height),
      _title(title),
      _isRunning(true),
      _inputManager(*this),
      _isViewportActive(false),
      _gui(width, height) {
  _window.Init(_width, _height, _title);
  _window.SetTargetFPS(60);

  rlImGuiSetup(true);

  _viewport = raylib::Rectangle(250, 40, _width - 550, _height - 30);

  _scene.load("./scene.json");
}

gui::Window::~Window() {
  rlImGuiShutdown();
  if (_isRunning) {
    _window.Close();
  }
}

void gui::Window::update() {
  _camera.update(_isViewportActive);
  _inputManager.handleInput();
}

void gui::Window::run() {
  while (_isRunning && !_window.ShouldClose()) {
    update();
    draw();
  }
}

bool gui::Window::isMouseInViewport() {
  raylib::Vector2 mousePosition = GetMousePosition();
  return CheckCollisionPointRec(mousePosition, _viewport);
}

void gui::Window::draw() {
  _window.BeginDrawing();

  raylib::Color backgroundColor(45, 45, 48);
  _window.ClearBackground(backgroundColor);

  DrawRectangleRec(_viewport, raylib::Color(135, 206, 235));

  drawViewport();

  rlImGuiBegin();

  _gui.drawInterface(_camera, *this);

  rlImGuiEnd();

  _window.EndDrawing();
}

void gui::Window::drawViewport() {
  BeginScissorMode(_viewport.x, _viewport.y, _viewport.width, _viewport.height);
  _camera.getCamera().BeginMode();
  DrawGrid(20, 1.0f);

  _scene.draw();

  _camera.getCamera().EndMode();

  _camera.getCamera().BeginMode();

  rlDrawRenderBatchActive();
  rlDisableDepthTest();

  _scene.drawGizmo();

  rlDrawRenderBatchActive();
  rlEnableDepthTest();

  _camera.getCamera().EndMode();

  EndScissorMode();

  if (isMouseInViewport() && !_isViewportActive) {
    raylib::Vector2 mousePos = GetMousePosition();
    bool isMousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool isMouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    _scene.handleGizmoInteraction(mousePos, isMousePressed, isMouseDown);
    if (!isMouseDown) {
      _scene.updateGizmo(_camera.getCamera(), mousePos);
    }
  }

  if (_isViewportActive) {
    DrawRectangleLinesEx(_viewport, 2, raylib::Color::Red());
  } else {
    DrawRectangleLinesEx(_viewport, 1, raylib::Color::Gray());
  }
}

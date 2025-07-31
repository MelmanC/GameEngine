#include "ViewportManager.hpp"
#include <raylib.h>

viewport::ViewportManager::ViewportManager(int windowWidth, int windowHeight)
    : _isActive(false) {
  updateViewportSize(windowWidth, windowHeight);
}

void viewport::ViewportManager::updateViewportSize(int windowWidth,
                                                   int windowHeight) {
  _viewport = raylib::Rectangle(250, 40, windowWidth - 550, windowHeight - 30);
}

void viewport::ViewportManager::setViewportBounds(int windowWidth,
                                                  int windowHeight) {
  updateViewportSize(windowWidth, windowHeight);
}

bool viewport::ViewportManager::isMouseInViewport(
    const raylib::Vector2& mousePos) const {
  return CheckCollisionPointRec(mousePos, _viewport);
}

void viewport::ViewportManager::setActive(bool active) {
  _isActive = active;
  if (_isActive) {
    SetMousePosition(_viewport.x + _viewport.width / 2,
                     _viewport.y + _viewport.height / 2);
    DisableCursor();
  } else {
    EnableCursor();
  }
}

void viewport::ViewportManager::handleGizmoInteraction(
    scene::Scene& scene, const raylib::Vector2& mousePos, bool isMousePressed,
    bool isMouseDown) {
  if (isMouseInViewport(mousePos) && !_isActive) {
    scene.handleGizmoInteraction(mousePos, isMousePressed, isMouseDown);
  }
}

void viewport::ViewportManager::updateGizmo(scene::Scene& scene,
                                            camera::Camera3D& camera,
                                            const raylib::Vector2& mousePos) {
  if (isMouseInViewport(mousePos) && !_isActive) {
    scene.updateGizmo(camera.getCamera(), mousePos);
  }
}

void viewport::ViewportManager::updateCameraControls(camera::Camera3D& camera) {
  camera.update(_isActive);
}

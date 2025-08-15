#include "Renderer.hpp"
#include "rlgl.h"

render::Renderer::Renderer(int width, int height)
    : _width(width), _height(height) {
}

void render::Renderer::beginFrame() {
  BeginDrawing();
  ClearBackground(_backgroundColor);
}

void render::Renderer::endFrame() {
  EndDrawing();
}

void render::Renderer::drawBackground(const raylib::Color& color) {
  ClearBackground(color);
}

void render::Renderer::drawViewport(const raylib::Rectangle& viewport,
                                    camera::CameraEditor& camera,
                                    ecs::RenderSystem* renderSystem,
                                    ecs::GizmoSystem* gizmoSystem) {
  DrawRectangleRec(viewport, _viewportColor);

  setupViewportScissor(viewport);

  camera.getCamera().BeginMode();
  drawGrid();

  renderSystem->render();

  camera.getCamera().EndMode();

  camera.getCamera().BeginMode();
  rlDrawRenderBatchActive();
  rlDisableDepthTest();
  gizmoSystem->drawGizmos();
  rlDrawRenderBatchActive();
  rlEnableDepthTest();
  camera.getCamera().EndMode();

  endViewportScissor();
}

void render::Renderer::drawViewportFrame(const raylib::Rectangle& viewport,
                                         bool isActive) {
  if (isActive) {
    DrawRectangleLinesEx(viewport, 2, raylib::Color::Red());
  } else {
    DrawRectangleLinesEx(viewport, 1, raylib::Color::Gray());
  }
}

void render::Renderer::setupViewportScissor(const raylib::Rectangle& viewport) {
  BeginScissorMode(viewport.x, viewport.y, viewport.width, viewport.height);
}

void render::Renderer::endViewportScissor() {
  EndScissorMode();
}

void render::Renderer::drawGrid() {
  DrawGrid(20, 2.0f);
}

void render::Renderer::setViewportSize(int width, int height) {
  _width = width;
  _height = height;
}

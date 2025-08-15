#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "CameraEditor.hpp"
#include "GizmoSystem.hpp"
#include "RenderSystem.hpp"

namespace scene {
  class Scene;
}  // namespace scene

namespace render {

  class Renderer {
    public:
      Renderer(int width, int height);
      ~Renderer() = default;

      void beginFrame();

      void endFrame();

      void drawViewport(const raylib::Rectangle& viewport,
                        camera::CameraEditor& camera,
                        ecs::RenderSystem* renderSystem,
                        ecs::GizmoSystem* gizmoSystem);

      void drawBackground(const raylib::Color& color);

      void drawViewportFrame(const raylib::Rectangle& viewport, bool isActive);

      void setViewportSize(int width, int height);

    private:
      void drawGrid();
      void setupViewportScissor(const raylib::Rectangle& viewport);
      void endViewportScissor();

      int _width;
      int _height;
      raylib::Color _backgroundColor{45, 45, 48};
      raylib::Color _viewportColor{135, 206, 235};
  };

}  // namespace render

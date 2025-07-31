#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "Camera3D.hpp"
#include "Scene.hpp"

namespace viewport {

  class ViewportManager {
    public:
      ViewportManager(int windowWidth, int windowHeight);
      ~ViewportManager() = default;

      void update(const raylib::Vector2& mousePos, bool isMousePressed,
                  bool isMouseDown);

      void updateCameraControls(camera::Camera3D& camera);

      bool isMouseInViewport(const raylib::Vector2& mousePos) const;

      void setActive(bool active);

      bool isActive() const {
        return _isActive;
      }

      const raylib::Rectangle& getViewport() const {
        return _viewport;
      }

      void setViewportBounds(int windowWidth, int windowHeight);

      void handleGizmoInteraction(scene::Scene& scene,
                                  const raylib::Vector2& mousePos,
                                  bool isMousePressed, bool isMouseDown);

      void updateGizmo(scene::Scene& scene, camera::Camera3D& camera,
                       const raylib::Vector2& mousePos);

    private:
      raylib::Rectangle _viewport;
      bool _isActive;

      void updateViewportSize(int windowWidth, int windowHeight);
  };

}  // namespace viewport

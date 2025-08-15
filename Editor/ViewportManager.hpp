#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "CameraEditor.hpp"
#include "ECSManager.hpp"
#include "GizmoSystem.hpp"
#include "Scene.hpp"
#include "SelectionSystem.hpp"

namespace viewport {

  class ViewportManager {
    public:
      ViewportManager(int windowWidth, int windowHeight);
      ~ViewportManager() = default;

      void update(const raylib::Vector2& mousePos, bool isMousePressed,
                  bool isMouseDown);

      void updateCameraControls(camera::CameraEditor& camera);

      bool isMouseInViewport(const raylib::Vector2& mousePos) const;

      void setActive(bool active);

      bool isActive() const {
        return _isActive;
      }

      const raylib::Rectangle& getViewport() const {
        return _viewport;
      }

      void setViewportBounds(int windowWidth, int windowHeight);

      void handleGizmoInteraction(const raylib::Vector2& mousePos,
                                  bool isMousePressed, bool isMouseDown,
                                  ecs::ECSManager* ecsManager,
                                  ecs::GizmoSystem* gizmoSystem,
                                  ecs::SelectionSystem* selectionSystem);

      void updateGizmo(camera::CameraEditor& camera,
                       const raylib::Vector2& mousePos,
                       ecs::ECSManager* ecsManager,
                       ecs::GizmoSystem* gizmoSystem,
                       ecs::SelectionSystem* selectionSystem);

    private:
      raylib::Rectangle _viewport;
      bool _isActive;

      void updateViewportSize(int windowWidth, int windowHeight);
  };

}  // namespace viewport

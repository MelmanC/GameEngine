#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "System.hpp"

namespace ecs {
  class ECSManager;
  enum class GizmoAxis;

  class GizmoSystem : public System {
    public:
      GizmoSystem() = default;

      void setECSManager(ECSManager* ecsManager) {
        _ecsManager = ecsManager;
      }

      void update(float deltaTime) override;

      void drawGizmos();

      void updateGizmo(Entity selectedEntity, const raylib::Camera3D& camera,
                       const raylib::Vector2& mousePos);

      void handleGizmoInteraction(Entity selectedEntity,
                                  const raylib::Vector2& mousePos,
                                  bool isMousePressed, bool isMouseDown);

      GizmoAxis getHoveredAxis(const raylib::Vector3& position,
                               const raylib::Camera3D& camera,
                               const raylib::Vector2& mousePos, float size,
                               float cubeSize) const;

    private:
      void drawTranslationGizmo(const raylib::Vector3& position,
                                GizmoAxis selectedAxis, float size,
                                float cubeSize, float lineThickness) const;

      ECSManager* _ecsManager = nullptr;
  };

}  // namespace ecs

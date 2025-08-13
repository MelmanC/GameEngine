#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>

namespace ecs {

  enum class GizmoMode {
    TRANSLATE,
    ROTATE,
    SCALE
  };

  enum class GizmoAxis {
    NONE,
    X_AXIS,
    Y_AXIS,
    Z_AXIS
  };

  struct GizmoComponent {
      GizmoMode mode = GizmoMode::TRANSLATE;

      GizmoAxis selectedAxis = GizmoAxis::NONE;

      bool isDragging = false;

      raylib::Vector2 lastMousePos{0, 0};

      raylib::Vector3 dragStartPos{0, 0, 0};

      float size = 1.5f;

      float coneSize = 0.1f;

      float lineThickness = 0.05f;
  };

}  // namespace ecs

#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>

namespace scene {
  class Scene;
}

namespace gizmo {

  enum class GizmoAxis {
    NONE,
    X_AXIS,
    Y_AXIS,
    Z_AXIS
  };

  enum class GizmoType {
    TRANSLATE,
    ROTATE,
    SCALE
  };

  class Gizmo {
    public:
      Gizmo() = default;
      ~Gizmo() = default;

      void drawGizmo(const raylib::Vector3& position) const;

      GizmoAxis getHoveredAxis(const raylib::Vector3& position,
                               const raylib::Camera3D& camera,
                               const raylib::Vector2& mousePos) const;

      void setSize(float size) {
        _size = size;
      }

      float getSize() const {
        return _size;
      }

      bool isAxisSelected() const {
        return _selectedAxis != GizmoAxis::NONE;
      }

      void setSelectedAxis(GizmoAxis axis) {
        _selectedAxis = axis;
      }

      void setMode(GizmoType mode) {
        _mode = mode;
      }

      GizmoType getMode() const {
        return _mode;
      }

      GizmoAxis getSelectedAxis() const {
        return _selectedAxis;
      }

      void drawTranslationGizmo(const raylib::Vector3& position) const;

      // void drawRotationGizmo(const raylib::Vector3& position) const;
      //
      // void drawScaleGizmo(const raylib::Vector3& position) const;

    private:
      GizmoAxis _selectedAxis = GizmoAxis::NONE;
      GizmoType _mode = GizmoType::TRANSLATE;
      float _size = 1.5f;
      float _cubeSize = 0.2f;
      float _lineThickness = 0.05f;
  };

}  // namespace gizmo

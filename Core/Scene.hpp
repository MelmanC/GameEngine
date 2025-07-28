#pragma once
#include <raylib.h>
#include <raylib-cpp.hpp>
#include "Gizmo.hpp"
#include "IGameShape.hpp"

namespace scene {
  class Scene {
    public:
      std::vector<std::unique_ptr<shape::IGameShape>> _shapes;

      Scene() = default;
      ~Scene() = default;

      void loadFromJson(const std::string& jsonFilePath);
      void saveToJson(const std::string& jsonFilePath) const;
      void draw() const;
      void setSelectedObject(int index);
      void addShape(std::unique_ptr<shape::IGameShape> shape);
      shape::IGameShape* getSelectedObject() const;
      void updateGizmo(const raylib::Camera3D& camera,
                       const raylib::Vector2& mousePos);
      void drawGizmo() const;

      void handleGizmoInteraction(const raylib::Vector2& mousePos,
                                  bool isMousePressed, bool isMouseDown);

    private:
      gizmo::Gizmo _gizmo;
      bool _isDragging = false;
      raylib::Vector2 _lastMousePos;
      raylib::Vector3 _dragStartPos;
  };
}  // namespace scene

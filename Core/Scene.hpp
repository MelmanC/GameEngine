#pragma once
#include <raylib.h>
#include <raylib-cpp.hpp>
#include "Gizmo.hpp"
#include "IGameShape.hpp"
#include "JsonHandler.hpp"

namespace scene {
  class Scene {
    public:
      Scene() = default;
      ~Scene() = default;

      void draw() const;
      void setSelectedObject(int index);
      void addShape(std::unique_ptr<shape::IGameShape> shape);
      void removeShape(int index);
      shape::IGameShape* getSelectedObject() const;
      void drawGizmo() const;

      void updateGizmo(const raylib::Camera3D& camera,
                       const raylib::Vector2& mousePos);

      void handleGizmoInteraction(const raylib::Vector2& mousePos,
                                  bool isMousePressed, bool isMouseDown);

      const std::vector<std::unique_ptr<shape::IGameShape>>& getShapes() const {
        return _shapes;
      }

      void clearShapes() {
        _shapes.clear();
      }

      void save(const std::string& jsonFilePath) const {
        _jsonHandler.saveToJson(*this, jsonFilePath);
      }

      void load(const std::string& jsonFilePath) {
        _jsonHandler.loadFromJson(*this, jsonFilePath);
      }

    private:
      std::vector<std::unique_ptr<shape::IGameShape>> _shapes;
      gizmo::Gizmo _gizmo;
      jsonfile::JsonHandler _jsonHandler;
  };
}  // namespace scene

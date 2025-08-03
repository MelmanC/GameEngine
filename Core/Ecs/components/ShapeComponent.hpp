#pragma once
#include <string>

namespace ecs {

  enum class ShapeType {
    CUBE,
    SPHERE,
    CYLINDER,
    PLANE,
  };

  struct ShapeComponent {
      ShapeType type = ShapeType::CUBE;

      std::string getShapeTypeString() const {
        switch (type) {
          case ShapeType::CUBE:
            return "Cube";
          case ShapeType::SPHERE:
            return "Sphere";
          case ShapeType::PLANE:
            return "Plane";
          case ShapeType::CYLINDER:
            return "Cylinder";
          default:
            return "Unknown";
        }
      }
  };

}  // namespace ecs

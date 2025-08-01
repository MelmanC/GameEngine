#pragma once

#include <string>
#include "Component.hpp"

namespace ecs {
  enum class ShapeType {
    CUBE,
    SPHERE,
    CYLINDER,
    PLANE,
  };

  class ShapeComponent : public Component<ShapeComponent> {
    public:
      ShapeComponent(ShapeType type = ShapeType::CUBE) : _type(type) {
      }

      ShapeType getType() const {
        return _type;
      }

      std::string getShapeTypeString() const {
        switch (_type) {
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

      void setType(ShapeType type) {
        _type = type;
      }

    private:
      ShapeType _type;
  };
}  // namespace ecs

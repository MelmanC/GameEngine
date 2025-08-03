#pragma once

#include "BoundingBox.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace ecs {

  struct TransformComponent {
      raylib::Vector3 position{0, 0, 0};
      raylib::Vector3 rotation{0, 0, 0};
  };

  struct CubeTransformComponent : public TransformComponent {
      raylib::Vector3 size{1, 1, 1};

      raylib::BoundingBox getBoundingBox() const {
        return raylib::BoundingBox{position - size / 2, position + size / 2};
      }
  };

  struct SphereTransformComponent : public TransformComponent {
      float radius{1.0f};

      raylib::BoundingBox getBoundingBox() const {
        return raylib::BoundingBox{
            position - raylib::Vector3{radius, radius, radius},
            position + raylib::Vector3{radius, radius, radius}};
      }
  };

  struct CylinderTransformComponent : public TransformComponent {
      float radius{1.0f};
      float height{1.0f};

      raylib::BoundingBox getBoundingBox() const {
        return raylib::BoundingBox{
            position - raylib::Vector3{radius, height / 2, radius},
            position + raylib::Vector3{radius, height / 2, radius}};
      }
  };

  struct PlaneTransformComponent : public TransformComponent {
      raylib::Vector2 size{1, 1};

      raylib::BoundingBox getBoundingBox() const {
        return raylib::BoundingBox{
            position - raylib::Vector3{size.x / 2, 0, size.y / 2},
            position + raylib::Vector3{size.x / 2, 0, size.y / 2}};
      }
  };
}  // namespace ecs

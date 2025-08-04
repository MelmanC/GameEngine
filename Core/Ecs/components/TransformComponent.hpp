#pragma once

#include "BoundingBox.hpp"
#include "Model.hpp"
#include "ModelComponent.hpp"
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

  struct ModelTransformComponent : public TransformComponent {
      raylib::Vector3 scale{1, 1, 1};

      raylib::BoundingBox getBoundingBox(const raylib::Model& model) const {
        raylib::BoundingBox modelBox = GetModelBoundingBox(model);
        raylib::Vector3 size = Vector3Subtract(modelBox.max, modelBox.min);
        raylib::Vector3 scaledSize = Vector3Multiply(size, scale);

        raylib::Vector3 center =
            Vector3Add(modelBox.min, Vector3Scale(size, 0.5f));
        raylib::Vector3 scaledCenter = Vector3Multiply(center, scale);
        raylib::Vector3 transformedCenter = Vector3Add(position, scaledCenter);

        return raylib::BoundingBox{
            Vector3Subtract(transformedCenter, Vector3Scale(scaledSize, 0.5f)),
            Vector3Add(transformedCenter, Vector3Scale(scaledSize, 0.5f))};
      }
  };
}  // namespace ecs

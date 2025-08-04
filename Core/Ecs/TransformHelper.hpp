#pragma once

#include "ECSManager.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

namespace ecs {
  class TransformHelper {
    public:
      static raylib::Vector3 getPosition(Entity entity,
                                         ECSManager* ecsManager) {
        auto& shape = ecsManager->getComponent<ShapeComponent>(entity);

        switch (shape.type) {
          case ShapeType::CUBE:
            return ecsManager->getComponent<CubeTransformComponent>(entity)
                .position;
          case ShapeType::SPHERE:
            return ecsManager->getComponent<SphereTransformComponent>(entity)
                .position;
          case ShapeType::CYLINDER:
            return ecsManager->getComponent<CylinderTransformComponent>(entity)
                .position;
          case ShapeType::PLANE:
            return ecsManager->getComponent<PlaneTransformComponent>(entity)
                .position;
          case ShapeType::MODEL:
            return ecsManager->getComponent<ModelTransformComponent>(entity)
                .position;
          default:
            return {0, 0, 0};
        }
      }

      static void setPosition(Entity entity, const raylib::Vector3& position,
                              ECSManager* ecsManager) {
        auto& shape = ecsManager->getComponent<ShapeComponent>(entity);

        switch (shape.type) {
          case ShapeType::CUBE:
            ecsManager->getComponent<CubeTransformComponent>(entity).position =
                position;
            break;
          case ShapeType::SPHERE:
            ecsManager->getComponent<SphereTransformComponent>(entity)
                .position = position;
            break;
          case ShapeType::CYLINDER:
            ecsManager->getComponent<CylinderTransformComponent>(entity)
                .position = position;
            break;
          case ShapeType::PLANE:
            ecsManager->getComponent<PlaneTransformComponent>(entity).position =
                position;
          case ShapeType::MODEL:
            ecsManager->getComponent<ModelTransformComponent>(entity).position =
                position;
            break;
        }
      }

      static void movePosition(Entity entity, const raylib::Vector3& offset,
                               ECSManager* ecsManager) {
        raylib::Vector3 currentPos = getPosition(entity, ecsManager);
        setPosition(entity, Vector3Add(currentPos, offset), ecsManager);
      }

      static raylib::Vector3 getRotation(Entity entity,
                                         ECSManager* ecsManager) {
        auto& shape = ecsManager->getComponent<ShapeComponent>(entity);

        switch (shape.type) {
          case ShapeType::CUBE:
            return ecsManager->getComponent<CubeTransformComponent>(entity)
                .rotation;
          case ShapeType::SPHERE:
            return ecsManager->getComponent<SphereTransformComponent>(entity)
                .rotation;
          case ShapeType::CYLINDER:
            return ecsManager->getComponent<CylinderTransformComponent>(entity)
                .rotation;
          case ShapeType::PLANE:
            return ecsManager->getComponent<PlaneTransformComponent>(entity)
                .rotation;
          case ShapeType::MODEL:
            return ecsManager->getComponent<ModelTransformComponent>(entity)
                .rotation;
          default:
            return {0, 0, 0};
        }
      }

      static void setRotation(Entity entity, const raylib::Vector3& rotation,
                              ECSManager* ecsManager) {
        auto& shape = ecsManager->getComponent<ShapeComponent>(entity);

        switch (shape.type) {
          case ShapeType::CUBE:
            ecsManager->getComponent<CubeTransformComponent>(entity).rotation =
                rotation;
            break;
          case ShapeType::SPHERE:
            ecsManager->getComponent<SphereTransformComponent>(entity)
                .rotation = rotation;
            break;
          case ShapeType::CYLINDER:
            ecsManager->getComponent<CylinderTransformComponent>(entity)
                .rotation = rotation;
            break;
          case ShapeType::PLANE:
            ecsManager->getComponent<PlaneTransformComponent>(entity).rotation =
                rotation;
          case ShapeType::MODEL:
            ecsManager->getComponent<ModelTransformComponent>(entity).rotation =
                rotation;
            break;
        }
      }

      static raylib::BoundingBox getBoundingBox(Entity entity,
                                                ECSManager* ecsManager) {
        auto& shape = ecsManager->getComponent<ShapeComponent>(entity);

        switch (shape.type) {
          case ShapeType::CUBE:
            return ecsManager->getComponent<CubeTransformComponent>(entity)
                .getBoundingBox();
          case ShapeType::SPHERE:
            return ecsManager->getComponent<SphereTransformComponent>(entity)
                .getBoundingBox();
          case ShapeType::CYLINDER:
            return ecsManager->getComponent<CylinderTransformComponent>(entity)
                .getBoundingBox();
          case ShapeType::PLANE:
            return ecsManager->getComponent<PlaneTransformComponent>(entity)
                .getBoundingBox();
          case ShapeType::MODEL: {
            auto& modelTransform =
                ecsManager->getComponent<ModelTransformComponent>(entity);

            auto& modelComponent =
                ecsManager->getComponent<ModelComponent>(entity);
            if (modelComponent.isLoaded && modelComponent.model) {
              return modelTransform.getBoundingBox(*modelComponent.model);
            }
          }
          default:
            return {{0, 0, 0}, {0, 0, 0}};
        }
      }

      static std::pair<float*, float*> getPositionAndRotationPtrs(
          Entity entity, ECSManager* ecsManager) {
        auto& shape = ecsManager->getComponent<ShapeComponent>(entity);

        switch (shape.type) {
          case ShapeType::CUBE: {
            auto& transform =
                ecsManager->getComponent<CubeTransformComponent>(entity);
            return {&transform.position.x, &transform.rotation.x};
          }
          case ShapeType::SPHERE: {
            auto& transform =
                ecsManager->getComponent<SphereTransformComponent>(entity);
            return {&transform.position.x, &transform.rotation.x};
          }
          case ShapeType::CYLINDER: {
            auto& transform =
                ecsManager->getComponent<CylinderTransformComponent>(entity);
            return {&transform.position.x, &transform.rotation.x};
          }
          case ShapeType::PLANE: {
            auto& transform =
                ecsManager->getComponent<PlaneTransformComponent>(entity);
            return {&transform.position.x, &transform.rotation.x};
          }
          case ShapeType::MODEL: {
            auto& transform =
                ecsManager->getComponent<ModelTransformComponent>(entity);
            return {&transform.position.x, &transform.rotation.x};
          }
          default:
            return {nullptr, nullptr};
        }
      }
  };
}  // namespace ecs

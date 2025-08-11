#include "RenderSystem.hpp"
#include <raylib.h>
#include <iostream>
#include "ECSManager.hpp"
#include "ModelComponent.hpp"
#include "RenderComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

void ecs::RenderSystem::update(float deltaTime) {
  (void)deltaTime;
  render();
}

void ecs::RenderSystem::render() {
  for (Entity entity : _entities) {
    if (!_ecsManager->hasComponent<ShapeComponent>(entity) ||
        !_ecsManager->hasComponent<RenderComponent>(entity)) {
      continue;
    }
    auto &shape = _ecsManager->getComponent<ShapeComponent>(entity);
    auto &render = _ecsManager->getComponent<RenderComponent>(entity);

    if (!render.visible)
      continue;

    switch (shape.type) {
      case ecs::ShapeType::CUBE: {
        auto &cubeTransform =
            _ecsManager->getComponent<CubeTransformComponent>(entity);
        Vector3 position = cubeTransform.position;
        Vector3 size = cubeTransform.size;
        Color color = render.color;
        DrawCube(position, size.x, size.y, size.z, color);
        break;
      }
      case ecs::ShapeType::SPHERE: {
        auto &sphereTransform =
            _ecsManager->getComponent<SphereTransformComponent>(entity);
        Vector3 position = sphereTransform.position;
        float radius = sphereTransform.radius;
        DrawSphere(position, radius, render.color);
        break;
      }
      case ecs::ShapeType::CYLINDER: {
        auto &cylinderTransform =
            _ecsManager->getComponent<CylinderTransformComponent>(entity);
        Vector3 position = cylinderTransform.position;
        float radius = cylinderTransform.radius;
        float height = cylinderTransform.height;
        DrawCylinder(position, radius, radius, height, 16, render.color);
        break;
      }
      case ecs::ShapeType::PLANE: {
        auto &planeTransform =
            _ecsManager->getComponent<PlaneTransformComponent>(entity);
        Vector3 position = planeTransform.position;
        Vector2 size = planeTransform.size;
        Color color = render.color;
        DrawPlane(position, size, color);
        break;
      }
      case ecs::ShapeType::MODEL: {
        auto &modelTransform =
            _ecsManager->getComponent<ModelTransformComponent>(entity);
        auto &modelComponent =
            _ecsManager->getComponent<ModelComponent>(entity);

        if (!modelComponent.isLoaded && !modelComponent.modelPath.empty()) {
          modelComponent.model = std::make_shared<raylib::Model>(
              LoadModel(modelComponent.modelPath.c_str()));
          modelComponent.isLoaded = true;
        }

        if (modelComponent.model) {
          Vector3 position = modelTransform.position;
          Vector3 scale = modelTransform.scale;
          Color color = render.color;

          // TODO : Handle rotation if needed
          DrawModel(*modelComponent.model, position, scale.x, color);
        }
        break;
      }
      default:
        std::cerr << "Unknown shape type!" << std::endl;
    }
  }
}

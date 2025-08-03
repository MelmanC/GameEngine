#include "RenderSystem.hpp"
#include <raylib.h>
#include <iostream>
#include "ECSManager.hpp"
#include "RenderComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

void ecs::RenderSystem::update(float deltaTime) {
  (void)deltaTime;
  render();
}

void ecs::RenderSystem::render() {
  for (Entity entity : _entities) {
    auto &shape = _ecsManager->getComponent<ShapeComponent>(entity);
    auto &render = _ecsManager->getComponent<RenderComponent>(entity);
    auto &transform = _ecsManager->getComponent<TransformComponent>(entity);

    if (!render.visible)
      continue;

    switch (shape.type) {
      case ecs::ShapeType::CUBE: {
        DrawCube(transform.position, transform.size.x, transform.size.y,
                 transform.size.z, render.color);
        break;
      }
      default:
        std::cerr << "Unknown shape type!" << std::endl;
    }
  }
}

#include "RenderSystem.hpp"
#include <raylib.h>
#include <iostream>
#include "RenderComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

void ecs::RenderSystem::update(float deltaTime) {
  (void)deltaTime;
  render();
}

void ecs::RenderSystem::render() {
  for (Entity entity : _entities) {
    auto &shape = _componentManager.getComponent<ShapeComponent>(entity);
    auto &transform =
        _componentManager.getComponent<TransformComponent>(entity);
    auto &render = _componentManager.getComponent<RenderComponent>(entity);

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

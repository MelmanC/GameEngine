#include "RenderSystem.hpp"
#include "RenderComponent.hpp"
#include "SelectionComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

void ecs::RenderSystem::update() {
}

void ecs::RenderSystem::render() {
  const auto &entities = _manager->getEntities();

  for (const auto &entity : entities) {
    if (!hasRequiredComponents(entity)) {
      continue;
    }

    auto *renderComponent = _manager->getComponent<RenderComponent>(entity);
    if (!renderComponent || !renderComponent->isVisible()) {
      continue;
    }

    auto *shapeComponent = _manager->getComponent<ShapeComponent>(entity);
    if (!shapeComponent) {
      continue;
    }

    switch (shapeComponent->getType()) {
      case ShapeType::CUBE: {
        renderCube(entity);
      };
      case ShapeType::SPHERE: {
        // renderSphere(entity);
      };
      case ShapeType::CYLINDER: {
        // renderCylinder(entity);
      };
      case ShapeType::PLANE: {
        // renderPlane(entity);
      };
      default:
        break;
    }

    auto *selectionComponent =
        _manager->getComponent<SelectionComponent>(entity);
    if (selectionComponent && selectionComponent->isSelected()) {
      renderSelectedOutline(entity);
    }
  }
}

void ecs::RenderSystem::renderCube(Entity entity) {
  auto *transformComp = _manager->getComponent<TransformComponent>(entity);
  auto *renderComp = _manager->getComponent<RenderComponent>(entity);

  DrawCube(transformComp->getPosition(), transformComp->getSize().x,
           transformComp->getSize().y, transformComp->getSize().z,
           renderComp->getColor());
}

void ecs::RenderSystem::renderSelectedOutline(Entity entity) {
  auto *transformComp = _manager->getComponent<TransformComponent>(entity);

  DrawCubeWires(transformComp->getPosition(), transformComp->getSize().x,
                transformComp->getSize().y, transformComp->getSize().z,
                raylib::Color::Red());
}

bool ecs::RenderSystem::hasRequiredComponents(Entity entity) const {
  return _manager->hasComponent<TransformComponent>(entity) &&
         _manager->hasComponent<RenderComponent>(entity) &&
         _manager->hasComponent<ShapeComponent>(entity);
}

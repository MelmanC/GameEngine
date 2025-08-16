#include "CameraEditorSystem.hpp"
#include "CameraComponent.hpp"
#include "EditorOnlyComponent.hpp"
#include "GizmoComponent.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "SelectionComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

void ecs::CameraEditorSystem::update(__attribute__((unused)) float deltaTime) {
  if (!_ecsManager)
    return;

  for (Entity entity : _entities) {
    if (_ecsManager->hasComponent<ecs::CameraComponent>(entity)) {
      if (_cameraToEditor.find(entity) == _cameraToEditor.end()) {
        Entity visualEntity = createCamera(entity);
        _cameraToEditor[entity] = visualEntity;
      }

      Entity visualEntity = _cameraToEditor[entity];
      updateVisualization(entity, visualEntity);
    }
  }

  auto it = _cameraToEditor.begin();

  while (it != _cameraToEditor.end()) {
    Entity cameraEntity = it->first;
    Entity visualEntity = it->second;

    if (!_ecsManager->hasComponent<ecs::CameraComponent>(cameraEntity)) {
      _ecsManager->destroyEntity(visualEntity);
      it = _cameraToEditor.erase(it);
    } else {
      ++it;
    }
  }
}

Entity ecs::CameraEditorSystem::createCamera(Entity cameraEntity) {
  if (!_ecsManager)
    return 0;

  auto& cameraComp =
      _ecsManager->getComponent<ecs::CameraComponent>(cameraEntity);
  Entity visualEntity = createCameraShape(cameraComp.position);
  return visualEntity;
}

void ecs::CameraEditorSystem::updateVisualization(Entity cameraEntity,
                                                  Entity visualEntity) {
  if (!_ecsManager)
    return;

  auto& cameraComp =
      _ecsManager->getComponent<ecs::CameraComponent>(cameraEntity);

  if (_ecsManager->hasComponent<ecs::CubeTransformComponent>(visualEntity)) {
    auto& transform =
        _ecsManager->getComponent<ecs::CubeTransformComponent>(visualEntity);
    transform.position = cameraComp.position;
  }
}

void ecs::CameraEditorSystem::removeCamera(Entity cameraEntity) {
  if (!_ecsManager)
    return;

  auto it = _cameraToEditor.find(cameraEntity);
  if (it != _cameraToEditor.end()) {
    Entity visualEntity = it->second;
    _ecsManager->destroyEntity(visualEntity);
    _cameraToEditor.erase(it);
  }
}

Entity ecs::CameraEditorSystem::createCameraShape(
    const raylib::Vector3& position) {
  Entity entity = _ecsManager->createEntity();

  ecs::CubeTransformComponent transform;
  transform.position = position;
  transform.size = raylib::Vector3(0.5f, 0.3f, 0.3f);
  _ecsManager->addComponent(entity, transform);

  _ecsManager->addComponent(
      entity, ecs::RenderComponent{raylib::Color::Purple(), true});
  _ecsManager->addComponent(entity, ecs::ShapeComponent{ecs::ShapeType::CUBE});
  _ecsManager->addComponent(
      entity, ecs::NameComponent{"Camera Visualization", "EditorCamera"});
  _ecsManager->addComponent(entity, ecs::EditorOnlyComponent{});
  _ecsManager->addComponent(entity, ecs::SelectionComponent{false});
  _ecsManager->addComponent(entity, ecs::GizmoComponent{});
  return entity;
}

Entity ecs::CameraEditorSystem::getCameraFromEditor(Entity visualEntity) const {
  for (const auto& pair : _cameraToEditor) {
    if (pair.second == visualEntity) {
      return pair.first;
    }
  }
  return 0;
}

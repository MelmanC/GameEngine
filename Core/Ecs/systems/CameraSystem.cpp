#include "CameraSystem.hpp"

void ecs::CameraSystem::update(float deltaTime) {
  (void)deltaTime;

  auto& cameraComponent =
      _ecsManager->getComponent<CameraComponent>(_activeCamera);

  raylib::Camera3D camera;

  camera.position = cameraComponent.position;
  camera.target = cameraComponent.target;
  camera.up = cameraComponent.up;
  camera.fovy = cameraComponent.fov;
  camera.projection = cameraComponent.projection;
}

void ecs::CameraSystem::setActiveCamera(Entity camera) {
  if (!_ecsManager->hasComponent<CameraComponent>(camera)) {
    throw std::runtime_error("Entity does not have a CameraComponent");
  }
  _activeCamera = camera;
}

raylib::Camera3D ecs::CameraSystem::getActiveCamera() const {
  if (!hasActiveCamera()) {
    throw std::runtime_error(
        "No active camera set or camera component missing");
  }

  const auto& cameraComponent =
      _ecsManager->getComponent<CameraComponent>(_activeCamera);

  raylib::Camera3D camera;
  camera.position = cameraComponent.position;
  camera.target = cameraComponent.target;
  camera.up = cameraComponent.up;
  camera.fovy = cameraComponent.fov;
  camera.projection = cameraComponent.projection;
  return camera;
}

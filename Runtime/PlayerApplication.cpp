#include "PlayerApplication.hpp"
#include <raylib.h>
#include "CameraComponent.hpp"
#include "ECSManager.hpp"
#include "GizmoComponent.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "ScriptComponent.hpp"
#include "SelectionComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

runtime::PlayerApplication::PlayerApplication(int width, int height,
                                              const std::string& name)
    : _PAwidth(width), _PAheight(height), _PAname(name) {
  _PAwindow.Init(_PAwidth, _PAheight, _PAname.c_str());
  _PAwindow.SetTargetFPS(60);

  initECS();
  _scene.initialize(nullptr, _ecsManager.get());
  loadScene("./scene.json");

  // Test Camera Component
  ecs::CameraComponent cameraComponent;
  cameraComponent.position = raylib::Vector3(0.0f, 5.0f, 10.0f);
  cameraComponent.target = raylib::Vector3(0.0f, 0.0f, 0.0f);
  cameraComponent.up = raylib::Vector3(0.0f, 1.0f, 0.0f);
  cameraComponent.fov = 45.0f;
  cameraComponent.projection = CAMERA_PERSPECTIVE;
  cameraComponent.mode = CAMERA_FIRST_PERSON;
  cameraComponent.isActive = true;
  auto cameraEntity = _ecsManager->createEntity();
  _ecsManager->addComponent<ecs::CameraComponent>(cameraEntity,
                                                  cameraComponent);
  _cameraSystem->setActiveCamera(cameraEntity);
}

void runtime::PlayerApplication::run() {
  while (_PAisRunning && !_PAwindow.ShouldClose()) {
    _scriptSystem->update(_PAwindow.GetFrameTime());
    _cameraSystem->update(_PAwindow.GetFrameTime());

    _PAwindow.BeginDrawing();
    _PAwindow.ClearBackground(RAYWHITE);

    if (_cameraSystem->hasActiveCamera()) {
      const auto& activeCamera = _cameraSystem->getActiveCamera();
      BeginMode3D(activeCamera);
      _renderSystem->render();
      EndMode3D();
    }

    _PAwindow.EndDrawing();
  }
}

void runtime::PlayerApplication::initECS() {
  _ecsManager = std::make_unique<ecs::ECSManager>();

  _ecsManager->registerComponent<ecs::CubeTransformComponent>();
  _ecsManager->registerComponent<ecs::SphereTransformComponent>();
  _ecsManager->registerComponent<ecs::CylinderTransformComponent>();
  _ecsManager->registerComponent<ecs::PlaneTransformComponent>();
  _ecsManager->registerComponent<ecs::ModelTransformComponent>();
  _ecsManager->registerComponent<ecs::ModelComponent>();
  _ecsManager->registerComponent<ecs::RenderComponent>();
  _ecsManager->registerComponent<ecs::ShapeComponent>();
  _ecsManager->registerComponent<ecs::NameComponent>();
  _ecsManager->registerComponent<ecs::SelectionComponent>();
  _ecsManager->registerComponent<ecs::GizmoComponent>();
  _ecsManager->registerComponent<ecs::ScriptComponent>();
  _ecsManager->registerComponent<ecs::CameraComponent>();

  _renderSystem = _ecsManager->registerSystem<ecs::RenderSystem>();
  _scriptSystem = _ecsManager->registerSystem<ecs::ScriptSystem>();
  _cameraSystem = _ecsManager->registerSystem<ecs::CameraSystem>();

  _renderSystem->setECSManager(_ecsManager.get());
  _scriptSystem->setECSManager(_ecsManager.get());
  _cameraSystem->setECSManager(_ecsManager.get());

  Signature renderSignature;
  renderSignature.set(_ecsManager->getComponentType<ecs::RenderComponent>());
  renderSignature.set(_ecsManager->getComponentType<ecs::ShapeComponent>());
  _ecsManager->setSystemeSignature<ecs::RenderSystem>(renderSignature);

  Signature scriptSignature;
  scriptSignature.set(_ecsManager->getComponentType<ecs::ScriptComponent>());
  _ecsManager->setSystemeSignature<ecs::ScriptSystem>(scriptSignature);

  Signature cameraSignature;
  cameraSignature.set(_ecsManager->getComponentType<ecs::CameraComponent>());
  _ecsManager->setSystemeSignature<ecs::CameraSystem>(cameraSignature);
}

void runtime::PlayerApplication::loadScene(const std::string& scenePath) {
  try {
    _scene.load(scenePath);
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to load scene: " + std::string(e.what()));
  }
}

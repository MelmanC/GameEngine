#include "PlayerApplication.hpp"
#include <raylib.h>
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
    : _PAwidth(width), _PAheight(height), _PAname(name), _camera() {
  _PAwindow.Init(_PAwidth, _PAheight, _PAname.c_str());
  _PAwindow.SetTargetFPS(60);

  initCamera();
  initECS();
  _scene.initialize(nullptr, _ecsManager.get());
  loadScene("./scene.json");
}

void runtime::PlayerApplication::initCamera() {
  _camera.position = raylib::Vector3(0, 1, 10);
  _camera.target = raylib::Vector3(0, 0, 1);
  _camera.up = raylib::Vector3(0, 1, 0);
  _camera.fovy = 45.0f;
  _camera.projection = CAMERA_PERSPECTIVE;
  DisableCursor();
}

void runtime::PlayerApplication::run() {
  while (_PAisRunning && !_PAwindow.ShouldClose()) {
    _camera.Update(CAMERA_FIRST_PERSON);

    _scriptSystem->update(_PAwindow.GetFrameTime());

    _PAwindow.BeginDrawing();
    _PAwindow.ClearBackground(RAYWHITE);

    _camera.BeginMode();

    _renderSystem->render();

    _camera.EndMode();

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

  _renderSystem = _ecsManager->registerSystem<ecs::RenderSystem>();
  _scriptSystem = _ecsManager->registerSystem<ecs::ScriptSystem>();

  _renderSystem->setECSManager(_ecsManager.get());
  _scriptSystem->setECSManager(_ecsManager.get());

  Signature renderSignature;
  renderSignature.set(_ecsManager->getComponentType<ecs::RenderComponent>());
  renderSignature.set(_ecsManager->getComponentType<ecs::ShapeComponent>());
  _ecsManager->setSystemeSignature<ecs::RenderSystem>(renderSignature);

  Signature scriptSignature;
  scriptSignature.set(_ecsManager->getComponentType<ecs::ScriptComponent>());
  _ecsManager->setSystemeSignature<ecs::ScriptSystem>(scriptSignature);
}

void runtime::PlayerApplication::loadScene(const std::string& scenePath) {
  try {
    _scene.load(scenePath);
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to load scene: " + std::string(e.what()));
  }
}

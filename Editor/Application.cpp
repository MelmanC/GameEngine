#include "Application.hpp"
#include <imgui.h>
#include <rlImGui.h>
#include <iostream>
#include "ECSManager.hpp"
#include "EntityManager.hpp"
#include "GizmoComponent.hpp"
#include "GizmoSystem.hpp"
#include "ModelComponent.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "SelectionComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

app::Application::Application(int width, int height, const char* title)
    : _width(width),
      _height(height),
      _title(title),
      _window(),
      _isRunning(true),
      _scene(),
      _camera(),
      _renderer(width, height),
      _viewportManager(width, height) {
  _window.Init(_width, _height, _title);
  _window.SetTargetFPS(60);

  rlImGuiSetup(true);

  initECS();

  _gui = std::make_unique<ui::Gui>(_width, _height);
  _inputManager = std::make_unique<input::InputManager>(*this);

  _scene.initialize(_selectionSystem.get(), _ecsManager.get());

  try {
    _scene.load("./scene.json");
  } catch (const std::exception& e) {
    std::cout << "Warning: Could not load scene.json: " << e.what()
              << std::endl;
  }
}

app::Application::~Application() {
  rlImGuiShutdown();
  if (_isRunning) {
    _window.Close();
  }
}

void app::Application::initECS() {
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

  _renderSystem = _ecsManager->registerSystem<ecs::RenderSystem>();
  _selectionSystem = _ecsManager->registerSystem<ecs::SelectionSystem>();
  _gizmoSystem = _ecsManager->registerSystem<ecs::GizmoSystem>();

  _renderSystem->setECSManager(_ecsManager.get());
  _selectionSystem->setECSManager(_ecsManager.get());
  _gizmoSystem->setECSManager(_ecsManager.get());

  Signature renderSignature;
  renderSignature.set(_ecsManager->getComponentType<ecs::RenderComponent>());
  renderSignature.set(_ecsManager->getComponentType<ecs::ShapeComponent>());
  _ecsManager->setSystemeSignature<ecs::RenderSystem>(renderSignature);

  Signature selectionSignature;
  selectionSignature.set(
      _ecsManager->getComponentType<ecs::SelectionComponent>());
  _ecsManager->setSystemeSignature<ecs::SelectionSystem>(selectionSignature);

  Signature gizmoSignature;
  gizmoSignature.set(_ecsManager->getComponentType<ecs::GizmoComponent>());
  gizmoSignature.set(_ecsManager->getComponentType<ecs::SelectionComponent>());
  _ecsManager->setSystemeSignature<ecs::GizmoSystem>(gizmoSignature);
}

void app::Application::run() {
  while (_isRunning && !_window.ShouldClose()) {
    update();
    render();
  }
}

void app::Application::shutdown() {
  _isRunning = false;
}

void app::Application::update() {
  _viewportManager.updateCameraControls(_camera);
  _inputManager->handleInput();

  float deltaTime = _window.GetFrameTime();
  _renderSystem->update(deltaTime);
  _selectionSystem->update(deltaTime);
  _gizmoSystem->update(deltaTime);
}

void app::Application::render() {
  _renderer.beginFrame();

  _renderer.drawBackground(raylib::Color(45, 45, 48));

  _renderer.drawViewport(_viewportManager.getViewport(), _camera,
                         _renderSystem.get(), _gizmoSystem.get());

  _renderer.drawViewportFrame(_viewportManager.getViewport(),
                              _viewportManager.isActive());

  rlImGuiBegin();
  _gui->drawInterface(_camera, *this);
  rlImGuiEnd();

  _renderer.endFrame();
}

raylib::Rectangle app::Application::getViewport() const {
  return _viewportManager.getViewport();
}

bool app::Application::isViewportActive() const {
  return _viewportManager.isActive();
}

void app::Application::setViewportActive(bool active) {
  _viewportManager.setActive(active);
}

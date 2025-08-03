#pragma once

#include <memory>
#include <raylib-cpp.hpp>
#include "Camera3D.hpp"
#include "ECSManager.hpp"
#include "GizmoSystem.hpp"
#include "Gui.hpp"
#include "InputManager.hpp"
#include "RenderSystem.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "SelectionSystem.hpp"
#include "ViewportManager.hpp"

namespace app {

  class Application {
    public:
      Application(int width, int height, const char* title);
      ~Application();

      void run();
      void shutdown();

      scene::Scene& getScene() {
        return _scene;
      }

      viewport::ViewportManager& getViewportManager() {
        return _viewportManager;
      }

      camera::Camera3D& getCamera() {
        return _camera;
      }

      ecs::ECSManager& getECSManager() {
        return *_ecsManager;
      }

      ecs::SelectionSystem& getSelectionSystem() {
        return *_selectionSystem;
      }

      raylib::Rectangle getViewport() const;
      bool isViewportActive() const;
      void setViewportActive(bool active);

      bool isRunning() const {
        return _isRunning;
      }

      void setRunning(bool running) {
        _isRunning = running;
      }

      int getWidth() const {
        return _width;
      }

      int getHeight() const {
        return _height;
      }

      ecs::GizmoSystem& getGizmoSystem() {
        return *_gizmoSystem;
      }

    private:
      void update();
      void render();
      void initECS();

      int _width, _height;
      const char* _title;
      raylib::Window _window;
      bool _isRunning;

      scene::Scene _scene;
      camera::Camera3D _camera;

      render::Renderer _renderer;
      viewport::ViewportManager _viewportManager;
      std::unique_ptr<ui::Gui> _gui;
      std::unique_ptr<input::InputManager> _inputManager;

      std::unique_ptr<ecs::ECSManager> _ecsManager;
      std::shared_ptr<ecs::RenderSystem> _renderSystem;
      std::shared_ptr<ecs::SelectionSystem> _selectionSystem;
      std::shared_ptr<ecs::GizmoSystem> _gizmoSystem;
  };

}  // namespace app

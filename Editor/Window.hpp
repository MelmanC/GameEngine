#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "Camera3D.hpp"
#include "Gui.hpp"
#include "Rectangle.hpp"
#include "Scene.hpp"

namespace gui {

  class Window {
    public:
      Window(int width, int height, const char* title);

      ~Window();

      void run();

      void draw();

      void updateCamera();

      void handleInput();

      void drawViewport();

      bool isMouseInViewport();

      raylib::Rectangle getViewport() const {
        return _viewport;
      }

      raylib::Camera3D& getCamera() {
        return _camera;
      }

      scene::Scene& getScene() {
        return _scene;
      }

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

      bool isViewportActive() const {
        return _isViewportActive;
      }

      void setViewportActive(bool active) {
        _isViewportActive = active;
      }

    private:
      int _width;
      int _height;
      const char* _title;
      bool _isRunning;
      raylib::Window _window;
      scene::Scene _scene;
      raylib::Camera3D _camera;

      raylib::Rectangle _viewport;
      bool _isViewportActive = false;
      ui::Gui _gui;
  };

}  // namespace gui

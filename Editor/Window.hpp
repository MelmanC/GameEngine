#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "Camera3D.hpp"
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

      void drawInterface();

      void drawViewport();

      bool isMouseInViewport();

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

      raylib::Rectangle _hierarchyPanel;
      raylib::Rectangle _propertiesPanel;
      raylib::Rectangle _toolbarPanel;
  };

}  // namespace gui

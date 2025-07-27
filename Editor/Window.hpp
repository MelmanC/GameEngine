#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "Camera3D.hpp"
#include "Scene.hpp"

namespace gui {

  class Window {
    public:
      Window(int width, int height, const char* title);

      ~Window();

      void run();

      void draw();

      void drawCamera();

      void updateCamera();

      void handleInput();

    private:
      int _width;
      int _height;
      const char* _title;
      bool _isRunning;
      raylib::Window _window;
      scene::Scene _scene;
      raylib::Camera3D _camera;
  };

}  // namespace gui

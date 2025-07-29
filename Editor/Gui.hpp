#pragma once

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>
#include <raylib-cpp.hpp>
#include "Camera3D.hpp"

namespace gui {
  class Window;
}

namespace ui {
  class Gui {
    public:
      Gui(const int width, const int height);

      ~Gui() = default;

      void drawInterface(camera::Camera3D &camera, gui::Window &window);

      void drawCameraInfo(camera::Camera3D &camera, gui::Window &window);

      void drawObjectInfo(gui::Window &window);

    private:
      raylib::Rectangle _hierarchyPanel;
      raylib::Rectangle _propertiesPanel;
      raylib::Rectangle _toolbarPanel;

      bool _showHierarchy = true;
      bool _showProperties = true;
  };
}  // namespace ui

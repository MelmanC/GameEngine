#pragma once

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>
#include <raylib-cpp.hpp>

namespace gui {
  class Window;
}

namespace ui {
  class Gui {
    public:
      Gui(const int width, const int height, gui::Window &window);

      ~Gui() = default;

      void drawInterface();

      void drawCameraInfo();

      void drawObjectInfo();

      bool isMouseInViewport();

    private:
      raylib::Rectangle _hierarchyPanel;
      raylib::Rectangle _propertiesPanel;
      raylib::Rectangle _toolbarPanel;

      bool _showHierarchy = true;
      bool _showProperties = true;
      gui::Window &_window;
  };
}  // namespace ui

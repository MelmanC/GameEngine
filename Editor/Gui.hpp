#pragma once

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>
#include <raylib-cpp.hpp>
#include "Camera3D.hpp"
#include "IGameShape.hpp"

namespace app {
  class Application;
}

namespace ui {
  class Gui {
    public:
      Gui(const int width, const int height);

      ~Gui() = default;

      void drawInterface(camera::Camera3D &camera, app::Application &app);

    private:
      void drawCameraInfo(camera::Camera3D &camera, app::Application &app);

      void drawObjectInfo(app::Application &app);

      void guiAlign(const char *label);

      void drawEntitiesInfos(shape::IGameShape *selectedObject);

      void drawTransformInfos(shape::IGameShape *selectedObject);

      void drawMaterialsInfos(shape::IGameShape *selectedObject);

      void drawMainMenuBar(app::Application &app);

      void drawHierarchyPanel(app::Application &app);

      void drawPropertiesPanel(camera::Camera3D &camera, app::Application &app);

      raylib::Rectangle _hierarchyPanel;
      raylib::Rectangle _propertiesPanel;
      raylib::Rectangle _toolbarPanel;

      bool _showHierarchy = true;
      bool _showProperties = true;
      bool _showEntities = true;
      bool _showTransform = true;
      bool _showMaterials = true;
  };
}  // namespace ui

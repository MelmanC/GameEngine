#pragma once

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>
#include <filesystem>
#include <raylib-cpp.hpp>
#include "Camera3D.hpp"
#include "ECSManager.hpp"
#include "EntityManager.hpp"

namespace app {
  class Application;
}

namespace ui {
  class Gui {
    public:
      Gui(const int width, const int height);

      ~Gui();

      void drawInterface(camera::Camera3D &camera, app::Application &app);

    private:
      void drawCameraInfo(camera::Camera3D &camera, app::Application &app);

      void drawObjectInfo(app::Application &app);

      void guiAlign(const char *label);

      void drawEntityInfo(Entity entity, ecs::ECSManager *ecsManager);

      void drawTransformInfo(Entity entity, ecs::ECSManager *ecsManager);

      void drawRenderInfo(Entity entity, ecs::ECSManager *ecsManager);

      void drawMainMenuBar(app::Application &app);

      void drawHierarchyPanel(app::Application &app);

      void drawPropertiesPanel(camera::Camera3D &camera, app::Application &app);

      void drawFinderPanel(app::Application &app);

      raylib::Rectangle _hierarchyPanel;
      raylib::Rectangle _propertiesPanel;
      raylib::Rectangle _toolbarPanel;
      // raylib::Rectangle _finderPanel;

      raylib::Texture2D _folderIcon;
      raylib::Texture2D _fileIcon;

      bool _showHierarchy = true;
      bool _showProperties = true;
      bool _showFinder = true;

      std::filesystem::path _selectedDirectory =
          std::filesystem::current_path();
      std::filesystem::path _selectedFile;

      bool _showEntities = true;
      bool _showTransform = true;
      bool _showMaterials = true;
  };
}  // namespace ui

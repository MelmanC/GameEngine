#pragma once

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>
#include <raylib-cpp.hpp>
#include "CameraEditor.hpp"
#include "FinderPanel.hpp"
#include "MainHierarchyPanel.hpp"
#include "MainMenuBar.hpp"
#include "MainPropertiesPanel.hpp"

namespace app {
  class Application;
}

namespace ui {

  class Gui {
    public:
      Gui(const int width, const int height);

      ~Gui();

      void drawInterface(camera::CameraEditor &camera, app::Application &app);

      void guiAlign(const char *label);

      raylib::Rectangle _hierarchyPanel;
      raylib::Rectangle _propertiesPanel;
      raylib::Rectangle _toolbarPanel;

      raylib::Texture2D _folderIcon;
      raylib::Texture2D _fileIcon;

      bool _showHierarchy = true;
      bool _showProperties = true;
      bool _showFinder = true;

      bool _showEntities = true;
      bool _showTransform = true;
      bool _showMaterials = true;
      bool _showScripts = true;

    private:
      MainMenuBar _mainMenuBar;
      MainHierarchyPanel _mainHierarchyPanel;
      MainPropertiesPanel _mainPropertiesPanel;
      FinderPanel _finderPanel;
  };
}  // namespace ui

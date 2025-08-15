#include "Gui.hpp"
#include "Application.hpp"
#include "Icons.h"
#include "MainMenuBar.hpp"
#include "Texture.hpp"
#include "imgui.h"

ui::Gui::Gui(const int width, const int height)
    : _hierarchyPanel(0, 40, 250, height - 40),
      _propertiesPanel(width - 300, 40, 300, height - 40),
      _toolbarPanel(0, 0, width, 40) {
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  _folderIcon = LoadTexture(FOLDER_ICON);
  _fileIcon = LoadTexture(FILE_ICON);
}

ui::Gui::~Gui() {
  UnloadTexture(_folderIcon);
}

void ui::Gui::guiAlign(const char *label) {
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::AlignTextToFramePadding();
  ImGui::Text("%s", label);
  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-1);
}

void ui::Gui::drawInterface(camera::CameraEditor &camera,
                            app::Application &app) {
  _mainMenuBar.draw(app, *this);
  _mainHierarchyPanel.draw(app, *this);
  _mainPropertiesPanel.draw(camera, app, *this);
  _finderPanel.draw(app, *this);
}

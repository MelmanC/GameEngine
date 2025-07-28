#include "Gui.hpp"
#include "Window.hpp"

ui::Gui::Gui(const int width, const int height, gui::Window &window)
    : _hierarchyPanel(0, 40, 250, height - 40),
      _propertiesPanel(width - 300, 40, 300, height - 40),
      _toolbarPanel(0, 0, width, 40),
      _window(window) {
  rlImGuiSetup(true);
}

void ui::Gui::drawInterface() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) {
      }
      if (ImGui::MenuItem("Open", "Ctrl+O")) {
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        _window.getScene().saveToJson("./scene.json");
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit")) {
        _window.setRunning(false);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      ImGui::MenuItem("Hierarchy", nullptr, &_showHierarchy);
      ImGui::MenuItem("Properties", nullptr, &_showProperties);
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  if (_showHierarchy) {
    ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, _window.getHeight() - 20),
                             ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Hierarchy", &_showHierarchy)) {
      for (size_t i = 0; i < _window.getScene()._shapes.size(); ++i) {
        const shape::GameShape &shape = _window.getScene()._shapes[i];
        std::string objectName =
            "Object " + std::to_string(i + 1) + ": " + shape.getName();

        if (ImGui::Selectable(objectName.c_str())) {
          _window.getScene().setSelectedObject(i);
        }
      }

      ImGui::Separator();
      if (ImGui::Button("Add Cube", ImVec2(-1, 0))) {
        _window.getScene()._shapes.push_back(
            shape::GameShape(2.0f, 2.0f, 2.0f, raylib::Vector3(10, 0, 0)));
        _window.getScene().saveToJson("./scene.json");
      }
    }
    ImGui::End();
  }

  if (_showProperties) {
    ImGui::SetNextWindowPos(ImVec2(_window.getWidth() - 300, 20),
                            ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, _window.getHeight() - 20),
                             ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Properties", &_showProperties)) {
      ImGui::Text("Camera Info:");
      ImGui::Text("Position: %.2f, %.2f, %.2f", _window.getCamera().position.x,
                  _window.getCamera().position.y,
                  _window.getCamera().position.z);
      ImGui::Text("Target: %.2f, %.2f, %.2f", _window.getCamera().target.x,
                  _window.getCamera().target.y, _window.getCamera().target.z);

      ImGui::Separator();
      ImGui::Text("Viewport Info:");
      ImGui::Text("Size: %.0fx%.0f", _window.getViewport().width,
                  _window.getViewport().height);
      ImGui::Text("Active: %s", _window.isViewportActive() ? "Yes" : "No");

      ImGui::Separator();
      ImGui::Text("Controls:");
      ImGui::BulletText("F - Toggle viewport control");
      ImGui::BulletText("ZQSD - Move camera");
      ImGui::BulletText("Space/Shift - Move up/down");
      ImGui::BulletText("ESC - Exit");
    }
    ImGui::End();
  }
}

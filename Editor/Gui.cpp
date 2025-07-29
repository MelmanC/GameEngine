#include "Gui.hpp"
#include "Cube.hpp"
#include "Window.hpp"

ui::Gui::Gui(const int width, const int height, gui::Window &window)
    : _hierarchyPanel(0, 40, 250, height - 40),
      _propertiesPanel(width - 300, 40, 300, height - 40),
      _toolbarPanel(0, 0, width, 40),
      _window(window) {
  rlImGuiSetup(true);
}

void ui::Gui::drawCameraInfo() {
  ImGui::Text("Camera Info:");
  ImGui::Text("Position: %.2f, %.2f, %.2f", _window.getCamera().position.x,
              _window.getCamera().position.y, _window.getCamera().position.z);
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

void ui::Gui::drawObjectInfo() {
  auto selectedObject = _window.getScene().getSelectedObject();
  if (selectedObject) {
    char name[64];
    std::strncpy(name, selectedObject->getName().c_str(), sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    if (ImGui::InputText("Name", name, sizeof(name))) {
      selectedObject->setName(name);
    }

    ImGui::Separator();

    raylib::Vector3 position = selectedObject->getPosition();
    ImGui::InputFloat3("Position", &position.x);
    selectedObject->setPosition(position);

    raylib::Color color = selectedObject->getColor();
    float colorArray[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
                           color.a / 255.0f};
    ImGui::ColorEdit4(
        "Color", colorArray,
        ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    selectedObject->setColor(
        raylib::Color(static_cast<unsigned char>(colorArray[0] * 255),
                      static_cast<unsigned char>(colorArray[1] * 255),
                      static_cast<unsigned char>(colorArray[2] * 255),
                      static_cast<unsigned char>(colorArray[3] * 255)));
    ImGui::Separator();
    float width = selectedObject->getWidth();
    float height = selectedObject->getHeight();
    float depth = selectedObject->getDepth();

    ImGui::InputFloat("Width", &width);
    ImGui::InputFloat("Height", &height);
    ImGui::InputFloat("Depth", &depth);
    selectedObject->setSize(raylib::Vector3(width, height, depth));

    if (ImGui::Button("Delete")) {
      _window.getScene().save("./scene.json");
    }
  }
}

void ui::Gui::drawInterface() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) {
      }
      if (ImGui::MenuItem("Open", "Ctrl+O")) {
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        _window.getScene().save("./scene.json");
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
      for (size_t i = 0; i < _window.getScene().getShapes().size(); ++i) {
        const auto &shape = _window.getScene().getShapes()[i];
        std::string objectName =
            "Object " + std::to_string(i + 1) + ": " + shape->getName();

        if (ImGui::Selectable(objectName.c_str())) {
          _window.getScene().setSelectedObject(i);
        }
      }

      ImGui::Separator();
      if (ImGui::Button("Add Cube", ImVec2(-1, 0))) {
        _window.getScene().addShape(std::make_unique<shape::Cube>(
            1.0f, 1.0f, 1.0f, raylib::Vector3(0.0f, 0.5f, 0.0f)));
        _window.getScene().save("./scene.json");
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
      if (!_window.getScene().getSelectedObject()) {
        drawCameraInfo();
      } else {
        drawObjectInfo();
      }
    }
    ImGui::End();
  }
}

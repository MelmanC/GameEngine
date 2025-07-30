#include "Gui.hpp"
#include "Cube.hpp"
#include "Window.hpp"

ui::Gui::Gui(const int width, const int height)
    : _hierarchyPanel(0, 40, 250, height - 40),
      _propertiesPanel(width - 300, 40, 300, height - 40),
      _toolbarPanel(0, 0, width, 40) {
  rlImGuiSetup(true);
}

void ui::Gui::drawCameraInfo(camera::Camera3D &camera, gui::Window &window) {
  ImGui::Text("Camera Info:");
  ImGui::Text("Position: %.2f, %.2f, %.2f", camera.getCamera().position.x,
              camera.getCamera().position.y, camera.getCamera().position.z);
  ImGui::Text("Target: %.2f, %.2f, %.2f", camera.getCamera().target.x,
              camera.getCamera().target.y, camera.getCamera().target.z);

  ImGui::Separator();
  ImGui::Text("Viewport Info:");
  ImGui::Text("Size: %.0fx%.0f", window.getViewport().width,
              window.getViewport().height);
  ImGui::Text("Active: %s", window.isViewportActive() ? "Yes" : "No");

  ImGui::Separator();
  ImGui::Text("Controls:");
  ImGui::BulletText("F - Toggle viewport control");
  ImGui::BulletText("ZQSD - Move camera");
  ImGui::BulletText("Space/Shift - Move up/down");
  ImGui::BulletText("ESC - Exit");
}

void ui::Gui::drawObjectInfo(gui::Window &window) {
  auto selectedObject = window.getScene().getSelectedObject();
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
      window.getScene().save("./scene.json");
    }
  }
}

void ui::Gui::drawInterface(camera::Camera3D &camera, gui::Window &window) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) {
      }
      if (ImGui::MenuItem("Open", "Ctrl+O")) {
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        window.getScene().save("./scene.json");
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit")) {
        window.setRunning(false);
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
    ImGui::SetNextWindowSize(ImVec2(250, window.getHeight() - 20),
                             ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Hierarchy", &_showHierarchy)) {
      for (size_t i = 0; i < window.getScene().getShapes().size(); ++i) {
        const auto &shape = window.getScene().getShapes()[i];
        std::string objectName =
            "Object " + std::to_string(i + 1) + ": " + shape->getName();

        if (ImGui::Selectable(objectName.c_str())) {
          window.getScene().setSelectedObject(i);
        }
      }

      ImGui::Separator();
      if (ImGui::Button("Add Cube", ImVec2(-1, 0))) {
        window.getScene().addShape(std::make_unique<shape::Cube>(
            1.0f, 1.0f, 1.0f, raylib::Vector3(0.0f, 0.5f, 0.0f)));
        window.getScene().save("./scene.json");
      }
    }
    ImGui::End();
  }

  if (_showProperties) {
    ImGui::SetNextWindowPos(ImVec2(window.getWidth() - 300, 20),
                            ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, window.getHeight() - 20),
                             ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Properties", &_showProperties)) {
      if (!window.getScene().getSelectedObject()) {
        drawCameraInfo(camera, window);
      } else {
        drawObjectInfo(window);
      }
    }
    ImGui::End();
  }
}

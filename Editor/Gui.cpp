#include "Gui.hpp"
#include "Cube.hpp"
#include "Window.hpp"
#include "imgui.h"

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

void ui::Gui::guiAlign(const char *label) {
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::AlignTextToFramePadding();
  ImGui::Text("%s", label);
  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-1);
}

void ui::Gui::drawEntitiesInfos(shape::IGameShape *selectedObject) {
  if (ImGui::CollapsingHeader(
          "Entity Info", _showEntities ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    char name[64];
    std::strncpy(name, selectedObject->getName().c_str(), sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    ImGui::Text("Name");
    ImGui::SameLine(80);
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputText("##EntityName", name, sizeof(name))) {
      selectedObject->setName(name);
    }

    ImGui::Text("Type");
    ImGui::SameLine(80);
    ImGui::Text("%s", selectedObject->getType().c_str());

    bool objectVisible = selectedObject->isVisible();
    ImGui::Text("Visible");
    ImGui::SameLine(80);
    ImGui::SetNextItemWidth(-1);
    if (ImGui::Checkbox("##Visible", &objectVisible)) {
      selectedObject->setVisible(!selectedObject->isVisible());
    }
    ImGui::Spacing();
  }
}

void ui::Gui::drawTransformInfos(shape::IGameShape *selectedObject) {
  if (ImGui::CollapsingHeader(
          "Transform Info",
          _showTransform ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    raylib::Vector3 position = selectedObject->getPosition();
    ImGui::Text("Position");
    ImGui::SameLine(80);
    ImGui::InputFloat3("##Position", &position.x);
    selectedObject->setPosition(position);

    raylib::Vector3 scale = selectedObject->getSize();
    ImGui::Text("Scale");
    ImGui::SameLine(80);
    ImGui::InputFloat3("##Scale", &scale.x);
    selectedObject->setSize(scale);

    ImGui::Spacing();
  }
}

void ui::Gui::drawMaterialsInfos(shape::IGameShape *selectedObject) {
  if (ImGui::CollapsingHeader(
          "Materials Info",
          _showMaterials ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    raylib::Color color = selectedObject->getColor();
    float colorArray[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
                           color.a / 255.0f};

    ImGui::Text("Color");
    ImGui::SameLine(80);
    ImGui::ColorEdit4(
        "##Color", colorArray,
        ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    selectedObject->setColor(
        raylib::Color(static_cast<unsigned char>(colorArray[0] * 255),
                      static_cast<unsigned char>(colorArray[1] * 255),
                      static_cast<unsigned char>(colorArray[2] * 255),
                      static_cast<unsigned char>(colorArray[3] * 255)));

    ImGui::Spacing();
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
      ImGui::Separator();
      if (ImGui::BeginMenu("Toggle Entities Info")) {
        ImGui::MenuItem("Entities", nullptr, &_showEntities);
        ImGui::MenuItem("Transform", nullptr, &_showTransform);
        ImGui::MenuItem("Materials", nullptr, &_showMaterials);
        ImGui::EndMenu();
      }
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
      auto selectedObject = window.getScene().getSelectedObject();
      if (!selectedObject) {
        drawCameraInfo(camera, window);
      } else {
        drawEntitiesInfos(selectedObject);
        drawTransformInfos(selectedObject);
        drawMaterialsInfos(selectedObject);
      }
    }
    ImGui::End();
  }
}

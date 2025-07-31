#include "Gui.hpp"
#include <iostream>
#include "Application.hpp"
#include "Cube.hpp"
#include "imgui.h"

ui::Gui::Gui(const int width, const int height)
    : _hierarchyPanel(0, 40, 250, height - 40),
      _propertiesPanel(width - 300, 40, 300, height - 40),
      _toolbarPanel(0, 0, width, 40) {
}

void ui::Gui::drawCameraInfo(camera::Camera3D &camera, app::Application &app) {
  ImGui::Text("Camera Info:");
  ImGui::Text("Position: %.2f, %.2f, %.2f", camera.getCamera().position.x,
              camera.getCamera().position.y, camera.getCamera().position.z);
  ImGui::Text("Target: %.2f, %.2f, %.2f", camera.getCamera().target.x,
              camera.getCamera().target.y, camera.getCamera().target.z);

  ImGui::Separator();
  ImGui::Text("Viewport Info:");
  ImGui::Text("Size: %.0fx%.0f", app.getViewport().width,
              app.getViewport().height);
  ImGui::Text("Active: %s", app.isViewportActive() ? "Yes" : "No");

  ImGui::Separator();
  ImGui::Text("Controls:");
  ImGui::BulletText("F - Toggle viewport control");
  ImGui::BulletText("ZQSD - Move camera");
  ImGui::BulletText("Space/Shift - Move up/down");
  ImGui::BulletText("ESC - Exit");
  ImGui::BulletText("Ctrl+S - Save scene");
  ImGui::BulletText("Ctrl+N - New scene");

  ImGui::Separator();
  ImGui::Text("Scene Stats:");
  ImGui::Text("Objects: %zu", app.getScene().getShapes().size());

  auto selectedObject = app.getScene().getSelectedObject();
  if (selectedObject) {
    ImGui::Text("Selected: %s", selectedObject->getName().c_str());
  } else {
    ImGui::Text("Selected: None");
  }
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

    if (ImGui::BeginTable("EntityTable", 2, ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
      guiAlign("Name");
      if (ImGui::InputText("##EntityName", name, sizeof(name))) {
        selectedObject->setName(name);
      }

      guiAlign("Type");
      ImGui::Text("%s", selectedObject->getType().c_str());

      guiAlign("Visible");
      bool objectVisible = selectedObject->isVisible();
      if (ImGui::Checkbox("##Visible", &objectVisible)) {
        selectedObject->setVisible(objectVisible);
      }

      ImGui::EndTable();
    }
    ImGui::Spacing();
  }
}

void ui::Gui::drawTransformInfos(shape::IGameShape *selectedObject) {
  if (ImGui::CollapsingHeader(
          "Transform Info",
          _showTransform ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    if (ImGui::BeginTable("TransformTable", 2,
                          ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
      raylib::Vector3 position = selectedObject->getPosition();
      guiAlign("Position");
      if (ImGui::InputFloat3("##Position", &position.x)) {
        selectedObject->setPosition(position);
      }

      raylib::Vector3 scale = selectedObject->getSize();
      guiAlign("Scale");
      if (ImGui::InputFloat3("##Scale", &scale.x)) {
        selectedObject->setSize(scale);
      }

      ImGui::EndTable();
    }
    ImGui::Spacing();
  }
}

void ui::Gui::drawMaterialsInfos(shape::IGameShape *selectedObject) {
  if (ImGui::CollapsingHeader(
          "Materials Info",
          _showMaterials ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    if (ImGui::BeginTable("MaterialsTable", 2,
                          ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
      raylib::Color color = selectedObject->getColor();
      float colorArray[4] = {color.r / 255.0f, color.g / 255.0f,
                             color.b / 255.0f, color.a / 255.0f};

      guiAlign("Color");
      if (ImGui::ColorEdit4(
              "##Color", colorArray,
              ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
        selectedObject->setColor(
            raylib::Color(static_cast<unsigned char>(colorArray[0] * 255),
                          static_cast<unsigned char>(colorArray[1] * 255),
                          static_cast<unsigned char>(colorArray[2] * 255),
                          static_cast<unsigned char>(colorArray[3] * 255)));
      }

      ImGui::EndTable();
    }
    ImGui::Spacing();
  }
}

void ui::Gui::drawMainMenuBar(app::Application &app) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) {
        app.getScene().clearShapes();
        std::cout << "New scene created!" << std::endl;
      }
      if (ImGui::MenuItem("Open", "Ctrl+O")) {
        try {
          app.getScene().load("./scene.json");
          std::cout << "Scene loaded successfully!" << std::endl;
        } catch (const std::exception &e) {
          std::cout << "Error loading scene: " << e.what() << std::endl;
        }
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        try {
          app.getScene().save("./scene.json");
          std::cout << "Scene saved successfully!" << std::endl;
        } catch (const std::exception &e) {
          std::cout << "Error saving scene: " << e.what() << std::endl;
        }
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit")) {
        app.setRunning(false);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      ImGui::MenuItem("Hierarchy", nullptr, &_showHierarchy);
      ImGui::MenuItem("Properties", nullptr, &_showProperties);
      ImGui::Separator();
      if (ImGui::BeginMenu("Panel Visibility")) {
        ImGui::MenuItem("Entity Info", nullptr, &_showEntities);
        ImGui::MenuItem("Transform Info", nullptr, &_showTransform);
        ImGui::MenuItem("Materials Info", nullptr, &_showMaterials);
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Create")) {
      if (ImGui::MenuItem("Cube")) {
        app.getScene().addShape(std::make_unique<shape::Cube>(
            1.0f, 1.0f, 1.0f, raylib::Vector3(0.0f, 0.5f, 0.0f)));
        std::cout << "Cube added to scene!" << std::endl;
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void ui::Gui::drawHierarchyPanel(app::Application &app) {
  if (!_showHierarchy)
    return;

  ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(250, app.getHeight() - 20),
                           ImGuiCond_FirstUseEver);

  if (ImGui::Begin("Hierarchy", &_showHierarchy)) {
    ImGui::Text("Scene Objects (%zu)", app.getScene().getShapes().size());
    ImGui::Separator();

    for (size_t i = 0; i < app.getScene().getShapes().size(); ++i) {
      const auto &shape = app.getScene().getShapes()[i];

      std::string objectName = shape->getName() + "##" + std::to_string(i);

      bool isSelected = shape->isSelected();
      if (ImGui::Selectable(objectName.c_str(), isSelected)) {
        app.getScene().setSelectedObject(static_cast<int>(i));
      }

      if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete")) {
          app.getScene().removeShape(i);
        }
        if (ImGui::MenuItem("Duplicate")) {
        }
        ImGui::Separator();
        bool visible = shape->isVisible();
        if (ImGui::MenuItem("Toggle Visibility")) {
          shape->setVisible(!visible);
        }
        ImGui::EndPopup();
      }
    }

    ImGui::Separator();
  }
  ImGui::End();
}

void ui::Gui::drawPropertiesPanel(camera::Camera3D &camera,
                                  app::Application &app) {
  if (!_showProperties)
    return;

  ImGui::SetNextWindowPos(ImVec2(app.getWidth() - 300, 20),
                          ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(300, app.getHeight() - 20),
                           ImGuiCond_FirstUseEver);

  if (ImGui::Begin("Properties", &_showProperties)) {
    auto selectedObject = app.getScene().getSelectedObject();
    if (!selectedObject) {
      drawCameraInfo(camera, app);
    } else {
      drawEntitiesInfos(selectedObject);
      drawTransformInfos(selectedObject);
      drawMaterialsInfos(selectedObject);

      ImGui::Separator();
    }
  }
  ImGui::End();
}

void ui::Gui::drawInterface(camera::Camera3D &camera, app::Application &app) {
  drawMainMenuBar(app);

  drawHierarchyPanel(app);
  drawPropertiesPanel(camera, app);
}

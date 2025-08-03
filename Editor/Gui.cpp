#include "Gui.hpp"
#include <iostream>
#include "3D/Shape.hpp"
#include "Application.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "SelectionComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"
#include "TransformHelper.hpp"
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
  int entityCount = 0;
  auto &ecsManager = app.getECSManager();
  ImGui::Text("Entities: %d", entityCount);

  if (app.getScene().hasSelectedEntity()) {
    Entity selectedEntity = app.getScene().getSelectedEntity();
    auto &name = ecsManager.getComponent<ecs::NameComponent>(selectedEntity);
    ImGui::Text("Selected: %s", name.name.c_str());
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

void ui::Gui::drawEntityInfo(Entity entity, ecs::ECSManager *ecsManager) {
  if (!ecsManager)
    return;

  if (ImGui::CollapsingHeader(
          "Entity Info", _showEntities ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    auto &nameComp = ecsManager->getComponent<ecs::NameComponent>(entity);
    auto &renderComp = ecsManager->getComponent<ecs::RenderComponent>(entity);

    char name[64];
    std::strncpy(name, nameComp.name.c_str(), sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    if (ImGui::BeginTable("EntityTable", 2, ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      guiAlign("Entity ID");
      ImGui::Text("%u", entity);

      guiAlign("Name");
      if (ImGui::InputText("##EntityName", name, sizeof(name))) {
        nameComp.name = name;
      }

      guiAlign("Type");
      ImGui::Text("%s", nameComp.type.c_str());

      guiAlign("Visible");
      if (ImGui::Checkbox("##Visible", &renderComp.visible)) {
      }

      ImGui::EndTable();
    }
    ImGui::Spacing();
  }
}

void ui::Gui::drawTransformInfo(Entity entity, ecs::ECSManager *ecsManager) {
  if (!ecsManager)
    return;

  if (ImGui::CollapsingHeader(
          "Transform Info",
          _showTransform ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    auto &shapeComp = ecsManager->getComponent<ecs::ShapeComponent>(entity);

    if (ImGui::BeginTable("TransformTable", 2,
                          ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      auto [posPtr, rotPtr] =
          ecs::TransformHelper::getPositionAndRotationPtrs(entity, ecsManager);

      if (posPtr && rotPtr) {
        guiAlign("Position");
        ImGui::InputFloat3("##Position", posPtr);

        guiAlign("Rotation");
        ImGui::InputFloat3("##Rotation", rotPtr);
      }

      switch (shapeComp.type) {
        case ecs::ShapeType::CUBE: {
          auto &cubeComp =
              ecsManager->getComponent<ecs::CubeTransformComponent>(entity);
          guiAlign("Size");
          ImGui::InputFloat3("##CubeSize", &cubeComp.size.x);
          break;
        }
        case ecs::ShapeType::SPHERE: {
          auto &sphereComp =
              ecsManager->getComponent<ecs::SphereTransformComponent>(entity);
          guiAlign("Radius");
          ImGui::InputFloat("##SphereRadius", &sphereComp.radius);
          break;
        }
        case ecs::ShapeType::CYLINDER: {
          auto &cylinderComp =
              ecsManager->getComponent<ecs::CylinderTransformComponent>(entity);
          guiAlign("Radius");
          ImGui::InputFloat("##CylinderRadius", &cylinderComp.radius);
          guiAlign("Height");
          ImGui::InputFloat("##CylinderHeight", &cylinderComp.height);
          break;
        }
        case ecs::ShapeType::PLANE: {
          auto &planeComp =
              ecsManager->getComponent<ecs::PlaneTransformComponent>(entity);
          guiAlign("Size");
          ImGui::InputFloat2("##PlaneSize", &planeComp.size.x);
          break;
        }
      }

      ImGui::EndTable();
    }
    ImGui::Spacing();
  }
}

void ui::Gui::drawRenderInfo(Entity entity, ecs::ECSManager *ecsManager) {
  if (!ecsManager)
    return;

  if (ImGui::CollapsingHeader(
          "Render Info", _showMaterials ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    auto &renderComp = ecsManager->getComponent<ecs::RenderComponent>(entity);

    if (ImGui::BeginTable("RenderTable", 2, ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      guiAlign("Color");
      float colorArray[4] = {
          renderComp.color.r / 255.0f, renderComp.color.g / 255.0f,
          renderComp.color.b / 255.0f, renderComp.color.a / 255.0f};

      if (ImGui::ColorEdit4(
              "##Color", colorArray,
              ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
        renderComp.color =
            raylib::Color(static_cast<unsigned char>(colorArray[0] * 255),
                          static_cast<unsigned char>(colorArray[1] * 255),
                          static_cast<unsigned char>(colorArray[2] * 255),
                          static_cast<unsigned char>(colorArray[3] * 255));
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
        app.getScene().clearEntities();
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
      shape3D::Shape shape;
      if (ImGui::MenuItem("Cube")) {
        shape.createCubeEntity({0.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f},
                               raylib::Color::White(), "New Cube",
                               &app.getECSManager());
      }
      if (ImGui::MenuItem("Sphere")) {
        shape.createSphereEntity({0.0f, 0.5f, 0.0f}, 1.0f,
                                 raylib::Color::White(), "New Sphere",
                                 &app.getECSManager());
      }
      if (ImGui::MenuItem("Cylinder")) {
        shape.createCylinderEntity({0.0f, 0.5f, 0.0f}, 1.0f, 2.0f,
                                   raylib::Color::White(), "New Cylinder",
                                   &app.getECSManager());
      }
      if (ImGui::MenuItem("Plane")) {
        shape.createPlaneEntity({0.0f, 0.0f, 0.0f}, {10.0f, 10.0f},
                                raylib::Color::White(), "New Plane",
                                &app.getECSManager());
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
    auto &ecsManager = app.getECSManager();

    std::vector<Entity> entities = app.getScene().getAllEntities();

    ImGui::Text("Scene Entities (%zu)", entities.size());
    ImGui::Separator();

    Entity entityToDelete = 0;
    bool shouldDelete = false;

    for (size_t i = 0; i < entities.size(); ++i) {
      Entity entity = entities[i];

      auto &nameComp = ecsManager.getComponent<ecs::NameComponent>(entity);
      auto &selectionComp =
          ecsManager.getComponent<ecs::SelectionComponent>(entity);

      std::string objectName = nameComp.name + "##" + std::to_string(entity);

      if (ImGui::Selectable(objectName.c_str(), selectionComp.selected)) {
        app.getScene().setSelectedEntity(entity);
      }

      if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete")) {
          if (app.getScene().getSelectedEntity() == entity) {
            app.getSelectionSystem().deselectAll();
          }
          entityToDelete = entity;
          shouldDelete = true;
        }
        if (ImGui::MenuItem("Duplicate")) {
          // TODO: Implement duplication logic
        }
        ImGui::Separator();
        auto &renderComp =
            ecsManager.getComponent<ecs::RenderComponent>(entity);
        if (ImGui::MenuItem("Toggle Visibility")) {
          renderComp.visible = !renderComp.visible;
        }
        ImGui::EndPopup();
      }
    }

    /* We delete the entity only after the loop to avoid
       invalidating iterators or references */
    if (shouldDelete) {
      ecsManager.destroyEntity(entityToDelete);
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
    auto &ecsManager = app.getECSManager();

    if (!app.getScene().hasSelectedEntity()) {
      drawCameraInfo(camera, app);
    } else {
      Entity selectedEntity = app.getScene().getSelectedEntity();

      drawEntityInfo(selectedEntity, &ecsManager);
      drawTransformInfo(selectedEntity, &ecsManager);
      drawRenderInfo(selectedEntity, &ecsManager);

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

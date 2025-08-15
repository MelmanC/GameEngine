#include "MainPropertiesPanel.hpp"
#include "Application.hpp"
#include "CameraEditorSystem.hpp"
#include "ECSManager.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "ScriptComponent.hpp"
#include "TransformHelper.hpp"

void ui::MainPropertiesPanel::draw(camera::CameraEditor &camera,
                                   app::Application &app, ui::Gui &gui) {
  if (!gui._showProperties)
    return;

  ImGui::SetNextWindowPos(ImVec2(app.getWidth() - 300, 20), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(300, app.getHeight() - 20), ImGuiCond_Always);

  if (ImGui::Begin("Properties", &gui._showProperties)) {
    auto &ecsManager = app.getECSManager();

    if (!app.getScene().hasSelectedEntity()) {
      drawCameraInfo(camera, app);
    } else {
      Entity selectedEntity = app.getScene().getSelectedEntity();

      auto linkedCamera = app.getECSManager()
                              .getSystem<ecs::CameraEditorSystem>()
                              ->getCameraFromEditor(selectedEntity);

      if (linkedCamera != 0) {
        drawCameraComponentInfo(selectedEntity, linkedCamera, &ecsManager, gui,
                                app);
      } else {
        drawEntityInfo(selectedEntity, &ecsManager, gui);
        drawTransformInfo(selectedEntity, &ecsManager, gui);
        drawRenderInfo(selectedEntity, &ecsManager, gui);
        drawScriptInfo(selectedEntity, &ecsManager, gui);
      }

      ImGui::Separator();
    }
  }
  ImGui::End();
}

void ui::MainPropertiesPanel::drawCameraComponentInfo(
    Entity visualEntity, Entity cameraEntity, ecs::ECSManager *ecsManager,
    ui::Gui &gui, app::Application &app) {
  if (!ecsManager)
    return;

  if (ImGui::CollapsingHeader(
          "Camera", gui._showCamera ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    auto &visualName =
        ecsManager->getComponent<ecs::NameComponent>(visualEntity);
    auto &cameraName =
        ecsManager->getComponent<ecs::NameComponent>(cameraEntity);

    if (ImGui::BeginTable("Camera", 2, ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      gui.guiAlign("Name");
      char name[64];
      std::strncpy(name, cameraName.name.c_str(), sizeof(name) - 1);
      name[sizeof(name) - 1] = '\0';
      if (ImGui::InputText("##Name", name, sizeof(name))) {
        ecsManager->getComponent<ecs::NameComponent>(cameraEntity).name = name;
        visualName.name = std::string(name) + " (Visualization)";
      }

      ImGui::EndTable();
    }
    ImGui::Spacing();
  }

  if (ImGui::CollapsingHeader("Camera Settings",
                              ImGuiTreeNodeFlags_DefaultOpen)) {
    auto &cameraComp =
        ecsManager->getComponent<ecs::CameraComponent>(cameraEntity);

    if (ImGui::BeginTable("CameraTable", 2, ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      gui.guiAlign("Position");
      if (ImGui::InputFloat3("##CameraPosition", &cameraComp.position.x)) {
      }

      gui.guiAlign("Target");
      ImGui::InputFloat3("##CameraTarget", &cameraComp.target.x);

      gui.guiAlign("Up");
      ImGui::InputFloat3("##CameraUp", &cameraComp.up.x);

      gui.guiAlign("FOV");
      ImGui::SliderFloat("##CameraFOV", &cameraComp.fov, 10.0f, 120.0f,
                         "%.1f°");

      gui.guiAlign("Projection");
      const char *projectionTypes[] = {"Perspective", "Orthographic"};
      int currentProjection = cameraComp.projection;
      if (ImGui::Combo("##CameraProjection", &currentProjection,
                       projectionTypes, 2)) {
        cameraComp.projection = (CameraProjection)currentProjection;
      }

      gui.guiAlign("Active");
      bool wasActive = cameraComp.isActive;
      if (ImGui::Checkbox("##CameraActive", &cameraComp.isActive)) {
        if (cameraComp.isActive && !wasActive) {
          auto entities = app.getScene().getAllEntities();
          for (Entity otherEntity : entities) {
            if (otherEntity != cameraEntity &&
                ecsManager->hasComponent<ecs::CameraComponent>(otherEntity)) {
              ecsManager->getComponent<ecs::CameraComponent>(otherEntity)
                  .isActive = false;
            }
          }
          app.getECSManager().getSystem<ecs::CameraSystem>()->setActiveCamera(
              cameraEntity);
        }
      }

      ImGui::EndTable();
    }

    ImGui::Spacing();

    if (ImGui::Button("Align Editor to Camera")) {
      auto &editorCamera = app.getCamera().getCamera();
      editorCamera.position = cameraComp.position;
      editorCamera.target = cameraComp.target;
      editorCamera.up = cameraComp.up;
      editorCamera.fovy = cameraComp.fov;
    }

    if (ImGui::Button("Align Camera to Editor")) {
      auto &editorCamera = app.getCamera().getCamera();
      cameraComp.position = editorCamera.position;
      cameraComp.target = editorCamera.target;
      cameraComp.up = editorCamera.up;
      cameraComp.fov = editorCamera.fovy;
    }

    ImGui::Spacing();

    drawScriptInfo(cameraEntity, ecsManager, gui);
  }
}

void ui::MainPropertiesPanel::drawRenderInfo(Entity entity,
                                             ecs::ECSManager *ecsManager,
                                             ui::Gui &gui) {
  if (!ecsManager)
    return;

  if (ImGui::CollapsingHeader(
          "Render Info",
          gui._showMaterials ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    auto &renderComp = ecsManager->getComponent<ecs::RenderComponent>(entity);

    if (ImGui::BeginTable("RenderTable", 2, ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      gui.guiAlign("Color");
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

void ui::MainPropertiesPanel::drawEntityInfo(Entity entity,
                                             ecs::ECSManager *ecsManager,
                                             ui::Gui &gui) {
  if (!ecsManager)
    return;

  if (ImGui::CollapsingHeader(
          "Entity Info",
          gui._showEntities ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    auto &nameComp = ecsManager->getComponent<ecs::NameComponent>(entity);
    auto &renderComp = ecsManager->getComponent<ecs::RenderComponent>(entity);

    char name[64];
    std::strncpy(name, nameComp.name.c_str(), sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    if (ImGui::BeginTable("EntityTable", 2, ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      gui.guiAlign("ID");
      ImGui::Text("%u", entity);

      gui.guiAlign("Name");
      if (ImGui::InputText("##EntityName", name, sizeof(name))) {
        nameComp.name = name;
      }

      gui.guiAlign("Type");
      ImGui::Text("%s", nameComp.type.c_str());

      gui.guiAlign("Visible");
      if (ImGui::Checkbox("##Visible", &renderComp.visible)) {
      }

      ImGui::EndTable();
    }
    ImGui::Spacing();
  }
}

void ui::MainPropertiesPanel::drawTransformInfo(Entity entity,
                                                ecs::ECSManager *ecsManager,
                                                ui::Gui &gui) {
  if (!ecsManager)
    return;

  if (ImGui::CollapsingHeader(
          "Transform Info",
          gui._showTransform ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    auto &shapeComp = ecsManager->getComponent<ecs::ShapeComponent>(entity);

    if (ImGui::BeginTable("TransformTable", 2,
                          ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      auto [posPtr, rotPtr] =
          ecs::TransformHelper::getPositionAndRotationPtrs(entity, ecsManager);

      if (posPtr && rotPtr) {
        gui.guiAlign("Position");
        ImGui::InputFloat3("##Position", posPtr);

        gui.guiAlign("Rotation");
        ImGui::InputFloat3("##Rotation", rotPtr);
      }

      switch (shapeComp.type) {
        case ecs::ShapeType::CUBE: {
          auto &cubeComp =
              ecsManager->getComponent<ecs::CubeTransformComponent>(entity);
          gui.guiAlign("Size");
          ImGui::InputFloat3("##CubeSize", &cubeComp.size.x);
          break;
        }
        case ecs::ShapeType::SPHERE: {
          auto &sphereComp =
              ecsManager->getComponent<ecs::SphereTransformComponent>(entity);
          gui.guiAlign("Radius");
          ImGui::InputFloat("##SphereRadius", &sphereComp.radius);
          break;
        }
        case ecs::ShapeType::CYLINDER: {
          auto &cylinderComp =
              ecsManager->getComponent<ecs::CylinderTransformComponent>(entity);
          gui.guiAlign("Radius");
          ImGui::InputFloat("##CylinderRadius", &cylinderComp.radius);
          gui.guiAlign("Height");
          ImGui::InputFloat("##CylinderHeight", &cylinderComp.height);
          break;
        }
        case ecs::ShapeType::PLANE: {
          auto &planeComp =
              ecsManager->getComponent<ecs::PlaneTransformComponent>(entity);
          gui.guiAlign("Size");
          ImGui::InputFloat2("##PlaneSize", &planeComp.size.x);
          break;
        }
        case ecs::ShapeType::MODEL: {
          auto &modelComp =
              ecsManager->getComponent<ecs::ModelTransformComponent>(entity);
          gui.guiAlign("Scale");
          if (ImGui::InputFloat3("##ModelScale", &modelComp.scale.x)) {
            modelComp.scale.y = modelComp.scale.x;
            modelComp.scale.z = modelComp.scale.x;
          }
          break;
        }
      }

      ImGui::EndTable();
    }
    ImGui::Spacing();
  }
}

void ui::MainPropertiesPanel::drawScriptInfo(Entity entity,
                                             ecs::ECSManager *ecsManager,
                                             ui::Gui &gui) {
  if (!ecsManager)
    return;

  if (ImGui::CollapsingHeader(
          "Script Info",
          gui._showScripts ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
    auto &scriptComp = ecsManager->getComponent<ecs::ScriptComponent>(entity);

    if (ImGui::BeginTable("ScriptTable", 2, ImGuiTableFlags_SizingFixedFit)) {
      ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      gui.guiAlign("Path");
      char scriptPath[256];
      std::strncpy(scriptPath, scriptComp.scriptPath.c_str(),
                   sizeof(scriptPath) - 1);
      scriptPath[sizeof(scriptPath) - 1] = '\0';
      if (ImGui::InputText("##ScriptPath", scriptPath, sizeof(scriptPath))) {
        scriptComp.scriptPath = scriptPath;
        scriptComp.state = ecs::ScriptState::NOT_LOADED;
        scriptComp.hasStarted = false;
      }

      gui.guiAlign("Enabled");
      if (ImGui::Checkbox("##ScriptEnabled", &scriptComp.enabled)) {
      }

      ImGui::EndTable();
    }
  }
}

void ui::MainPropertiesPanel::drawCameraInfo(camera::CameraEditor &camera,
                                             app::Application &app) {
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
  int entityCount = app.getEntityCount();
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

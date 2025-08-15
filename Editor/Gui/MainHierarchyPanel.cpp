#include "MainHierarchyPanel.hpp"
#include "Application.hpp"
#include "CameraEditorSystem.hpp"
#include "ECSManager.hpp"
#include "EditorOnlyComponent.hpp"
#include "EntityManager.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "SelectionComponent.hpp"

void ui::MainHierarchyPanel::draw(app::Application &app, ui::Gui &gui) {
  if (!gui._showHierarchy)
    return;

  ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(250, app.getHeight() - 20),
                           ImGuiCond_FirstUseEver);

  if (ImGui::Begin("Hierarchy", &gui._showHierarchy)) {
    auto &ecsManager = app.getECSManager();

    std::vector<Entity> entities = app.getScene().getAllEntities();

    Entity entityToDelete = 0;
    bool shouldDelete = false;
    Entity cameraToDelete = 0;
    bool isCamera = false;

    if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
      for (size_t i = 0; i < entities.size(); ++i) {
        Entity entity = entities[i];

        if (ecsManager.hasComponent<ecs::CameraComponent>(entity) &&
            !ecsManager.hasComponent<ecs::EditorOnlyComponent>(entity)) {
          continue;
        }

        auto &nameComp = ecsManager.getComponent<ecs::NameComponent>(entity);
        std::string displayName = nameComp.name;

        auto linkedCamera = app.getECSManager()
                                .getSystem<ecs::CameraEditorSystem>()
                                ->getCameraFromEditor(entity);

        if (linkedCamera != 0) {
          auto &linkedName =
              ecsManager.getComponent<ecs::NameComponent>(linkedCamera);

          displayName = linkedName.name;
        }

        std::string objectName = displayName + "##" + std::to_string(entity);

        if (ecsManager.hasComponent<ecs::SelectionComponent>(entity)) {
          auto &selectionComp =
              ecsManager.getComponent<ecs::SelectionComponent>(entity);
          if (ImGui::Selectable(objectName.c_str(), selectionComp.selected)) {
            app.getScene().setSelectedEntity(entity);
          }
        }
        if (ImGui::BeginPopupContextItem()) {
          Entity linkedCamera = app.getECSManager()
                                    .getSystem<ecs::CameraEditorSystem>()
                                    ->getCameraFromEditor(entity);
          bool isCameraEditor = (linkedCamera != 0);

          if (ImGui::MenuItem("Delete")) {
            if (app.getScene().getSelectedEntity() == entity) {
              app.getSelectionSystem().deselectAll();
            }

            if (isCameraEditor) {
              entityToDelete = entity;
              cameraToDelete = linkedCamera;
              isCamera = true;
            } else {
              entityToDelete = entity;
              shouldDelete = true;
            }
          }
          if (ImGui::MenuItem("Duplicate")) {
            // TODO: Implement duplication logic
          }
          if (ecsManager.hasComponent<ecs::RenderComponent>(entity)) {
            auto &renderComp =
                ecsManager.getComponent<ecs::RenderComponent>(entity);
            if (ImGui::MenuItem("Toggle Visibility")) {
              renderComp.visible = !renderComp.visible;
            }
          }
          ImGui::EndPopup();
        }
      }
      ImGui::TreePop();
    }

    if (isCamera) {
      auto system = app.getECSManager().getSystem<ecs::CameraEditorSystem>();

      system->removeCamera(cameraToDelete);
      ecsManager.destroyEntity(cameraToDelete);
    } else if (shouldDelete) {
      ecsManager.destroyEntity(entityToDelete);
    }
  }
  ImGui::End();
}

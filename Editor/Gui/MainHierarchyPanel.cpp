#include "MainHierarchyPanel.hpp"
#include "Application.hpp"
#include "ECSManager.hpp"
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

    if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
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
          auto &renderComp =
              ecsManager.getComponent<ecs::RenderComponent>(entity);
          if (ImGui::MenuItem("Toggle Visibility")) {
            renderComp.visible = !renderComp.visible;
          }
          ImGui::EndPopup();
        }
      }
      ImGui::TreePop();
    }

    /* We delete the entity only after the loop to avoid
       invalidating iterators or references */
    if (shouldDelete) {
      ecsManager.destroyEntity(entityToDelete);
    }
  }
  ImGui::End();
}

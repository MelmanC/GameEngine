#pragma once

#include "CameraEditor.hpp"
#include "ECSManager.hpp"
#include "EntityManager.hpp"

namespace app {
  class Application;
}

namespace ui {
  class Gui;

  class MainPropertiesPanel {
    public:
      MainPropertiesPanel() = default;

      ~MainPropertiesPanel() = default;

      void draw(camera::CameraEditor &camera, app::Application &app,
                ui::Gui &gui);

      void drawEntityInfo(Entity entity, ecs::ECSManager *ecsManager,
                          ui::Gui &gui);

      void drawTransformInfo(Entity entity, ecs::ECSManager *ecsManager,
                             ui::Gui &gui);

      void drawRenderInfo(Entity entity, ecs::ECSManager *ecsManager,
                          ui::Gui &gui);

      void drawScriptInfo(Entity entity, ecs::ECSManager *ecsManager,
                          ui::Gui &gui);

      void drawCameraComponentInfo(Entity visualEntity, Entity cameraEntity,
                                   ecs::ECSManager *ecsManager, ui::Gui &gui,
                                   app::Application &app);

      void drawCameraInfo(camera::CameraEditor &camera, app::Application &app);
  };
}  // namespace ui

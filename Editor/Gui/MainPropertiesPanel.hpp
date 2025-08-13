#pragma once

#include "Camera3D.hpp"
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

      void draw(camera::Camera3D &camera, app::Application &app, ui::Gui &gui);

      void drawEntityInfo(Entity entity, ecs::ECSManager *ecsManager,
                          ui::Gui &gui);

      void drawTransformInfo(Entity entity, ecs::ECSManager *ecsManager,
                             ui::Gui &gui);

      void drawRenderInfo(Entity entity, ecs::ECSManager *ecsManager,
                          ui::Gui &gui);

      void drawScriptInfo(Entity entity, ecs::ECSManager *ecsManager,
                          ui::Gui &gui);

      void drawCameraInfo(camera::Camera3D &camera, app::Application &app);
  };
}  // namespace ui

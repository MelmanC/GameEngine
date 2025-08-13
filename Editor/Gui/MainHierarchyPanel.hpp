#pragma once

namespace app {
  class Application;
}

namespace ui {

  class Gui;

  class MainHierarchyPanel {
    public:
      MainHierarchyPanel() = default;

      ~MainHierarchyPanel() = default;

      void draw(app::Application &app, ui::Gui &gui);
  };
}  // namespace ui

#pragma once

namespace app {
  class Application;
}

namespace ui {

  class Gui;

  class MainMenuBar {
    public:
      void draw(app::Application &app, ui::Gui &gui);
  };
}  // namespace ui

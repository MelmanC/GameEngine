#pragma once

#include <filesystem>

namespace app {
  class Application;
}

namespace ui {
  class Gui;

  class FinderPanel {
    public:
      FinderPanel() = default;

      ~FinderPanel() = default;

      void draw(app::Application &app, ui::Gui &gui);

    private:
      std::filesystem::path _selectedDirectory =
          std::filesystem::current_path();
      std::filesystem::path _selectedFile;
  };

}  // namespace ui

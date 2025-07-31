#pragma once

#include <raylib-cpp.hpp>
#include "Application.hpp"

namespace gui {

  class Window {
    public:
      Window(int width, int height, const char* title);
      ~Window();

      void run();

      bool shouldClose() const;
      void close();

    private:
      app::Application _application;
  };

}  // namespace gui

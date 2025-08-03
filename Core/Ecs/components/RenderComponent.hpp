#pragma once

#include "Color.hpp"

namespace ecs {

  struct RenderComponent {
      raylib::Color color = raylib::Color::White();
      bool visible = true;
  };

}  // namespace ecs

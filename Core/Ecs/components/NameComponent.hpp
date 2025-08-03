#pragma once

#include <string>

namespace ecs {

  struct NameComponent {
      std::string name = "Entity";
      std::string type = "Unknown";
  };

}  // namespace ecs

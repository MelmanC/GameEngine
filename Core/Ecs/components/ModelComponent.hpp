#pragma once

#include "Model.hpp"
namespace ecs {

  struct ModelComponent {
      std::string modelPath;

      std::shared_ptr<raylib::Model> model;

      bool isLoaded = false;

      int meshCount = 0;

      int materialCount = 0;
  };
}  // namespace ecs

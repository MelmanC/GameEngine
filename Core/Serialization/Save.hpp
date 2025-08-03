#pragma once

#include <nlohmann/json.hpp>
#include <raylib-cpp.hpp>

namespace scene {
  class Scene;
}

namespace ecs {
  class ECSManager;
}

namespace jsonfile {

  class Save {
    public:
      Save() = default;
      ~Save() = default;

      void saveToJson(const scene::Scene& scene,
                      const std::string& jsonFilePath,
                      ecs::ECSManager* ecsManager) const;
  };
}  // namespace jsonfile

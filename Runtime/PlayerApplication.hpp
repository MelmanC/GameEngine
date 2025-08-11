#pragma once

#include <atomic>
#include <raylib-cpp.hpp>
#include <string>
#include "Camera3D.hpp"
#include "RenderSystem.hpp"
#include "Scene.hpp"

namespace runtime {

  class PlayerApplication {
    public:
      PlayerApplication(int width, int height, const std::string& name);

      ~PlayerApplication() = default;

      void run();

      void initECS();

      int getWidth() const {
        return _PAwidth;
      }
      int getHeight() const {
        return _PAheight;
      }
      const std::string& getName() const {
        return _PAname;
      }

      void loadScene(const std::string& scenePath);

    private:
      int _PAwidth;
      int _PAheight;
      std::string _PAname;
      std::atomic<bool> _PAisRunning{true};
      raylib::Window _PAwindow;
      scene::Scene _scene;

      std::unique_ptr<ecs::ECSManager> _ecsManager;
      std::shared_ptr<ecs::RenderSystem> _renderSystem;
  };
}  // namespace runtime

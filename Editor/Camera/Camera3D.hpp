#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>

namespace camera {

  class Camera3D {
    public:
      Camera3D();
      ~Camera3D() = default;

      void update(bool isViewportActive);

      raylib::Camera3D& getCamera() {
        return _camera;
      }

      void handleMovementInput(int forwardKey, int backwardKey, int leftKey,
                               int rightKey);
      void handleMouseLook();

    private:
      float _speed = 5.0f;
      raylib::Camera3D _camera;
  };
}  // namespace camera

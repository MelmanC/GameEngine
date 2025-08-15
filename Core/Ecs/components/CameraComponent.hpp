#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>

namespace ecs {

  struct CameraComponent {
      raylib::Vector3 position{0, 0, 0};
      raylib::Vector3 target{0, 0, -1};
      raylib::Vector3 up{0, 1, 0};

      float fov = 45.0f;

      CameraProjection projection = CAMERA_PERSPECTIVE;

      CameraMode mode = CAMERA_FIRST_PERSON;
      bool isActive = false;
  };

}  // namespace ecs

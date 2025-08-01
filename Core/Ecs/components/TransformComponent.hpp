#pragma once

#include "BoundingBox.hpp"
#include "Component.hpp"
#include "Vector3.hpp"

namespace ecs {

  class TransformComponent : public Component<TransformComponent> {
    public:
      TransformComponent(const raylib::Vector3 &position = {0, 0, 0},
                         const raylib::Vector3 &size = {1, 1, 1})
          : _position(position), _size(size) {
      }

      const raylib::Vector3 &getPosition() const {
        return _position;
      }
      const raylib::Vector3 &getSize() const {
        return _size;
      }

      void setPosition(const raylib::Vector3 &position) {
        _position = position;
      }
      void setSize(const raylib::Vector3 &size) {
        _size = size;
      }

      raylib::BoundingBox getBoundingBox() const {
        return raylib::BoundingBox{_position - _size / 2,
                                   _position + _size / 2};
      }

    private:
      raylib::Vector3 _position;
      raylib::Vector3 _size;
  };
}  // namespace ecs

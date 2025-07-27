#include "GameShape.hpp"

shape::GameShape::GameShape(float width, float height, float depth,
                            const raylib::Vector3& position)
    : _position(position),
      _size(width, height, depth),
      _color(raylib::Color::White()) {
}

void shape::GameShape::draw() const {
  // Dessiner un cube 3D
  DrawCube(_position, _size.x, _size.y, _size.z, _color);
  DrawCubeWires(_position, _size.x, _size.y, _size.z, raylib::Color::Black());
}

raylib::Vector3 shape::GameShape::getPosition() const {
  return _position;
}

raylib::Vector3 shape::GameShape::getSize() const {
  return _size;
}

float shape::GameShape::getWidth() const {
  return _size.x;
}

float shape::GameShape::getHeight() const {
  return _size.y;
}

float shape::GameShape::getDepth() const {
  return _size.z;
}

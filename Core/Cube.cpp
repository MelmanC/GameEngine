#include "Cube.hpp"

shape::Cube::Cube(float width, float height, float depth,
                  const raylib::Vector3& position) {
  _position = position;
  _size = raylib::Vector3(width, height, depth);
  _color = raylib::Color::White();
  _name = "Cube";
  _type = "Cube";
  _isSelected = false;
}

void shape::Cube::draw() const {
  DrawCube(_position, _size.x, _size.y, _size.z, _color);
  if (_isSelected) {
    DrawCubeWires(_position, _size.x, _size.y, _size.z, raylib::Color::Red());
  } else {
    DrawCubeWires(_position, _size.x, _size.y, _size.z, raylib::Color::Black());
  }
}

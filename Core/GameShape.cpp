#include "GameShape.hpp"

shape::GameShape::GameShape(float width, float height, float depth,
                            const raylib::Vector3& position)
    : _position(position),
      _size(width, height, depth),
      _color(raylib::Color::White()) {
}

void shape::GameShape::draw() const {
  DrawCube(_position, _size.x, _size.y, _size.z, _color);
  if (_isSelected) {
    DrawCubeWires(_position, _size.x, _size.y, _size.z, raylib::Color::Red());
  } else {
    DrawCubeWires(_position, _size.x, _size.y, _size.z, raylib::Color::Black());
  }
}

raylib::Color shape::GameShape::getColor() const {
  return _color;
}

std::string shape::GameShape::getName() const {
  return _name;
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

void shape::GameShape::setSelected(bool isSelected) {
  _isSelected = isSelected;
}

bool shape::GameShape::isSelected() const {
  return _isSelected;
}

#include <iostream>
#include "Window.hpp"

int main() {
  try {
    gui::Window window(1920, 1080, "Raylib C++ Engine");
    window.run();
  } catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
    return -1;
  }
  return 0;
}

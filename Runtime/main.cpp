#include <iostream>
#include "PlayerApplication.hpp"

int main(int argc, char** argv) {
  try {
    std::string name = argc > 1 ? argv[1] : "Default Name";
    runtime::PlayerApplication window(800, 600, name);
    window.run();
  } catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
    return -1;
  }
  return 0;
}

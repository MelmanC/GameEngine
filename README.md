# rlEngine

rlEngine is a small engine built on top of [raylib](https://www.raylib.com/) (cpp wrapper) to help you create games in C++.

## WARNING
At this moment, the engine is an editor that allows you to create scenes, add entities, and edit their properties.
The goal is to create a game engine that allows you to create games without writing code, but for now, it is just an editor.
Feel free to contribute to the project.

# Features
- 

# Prerequisites
- CMake 3.15 or higher 
- C++20
- [raylib](https://www.raylib.com/)
- [nlohmann/json](https://github.com/nlohmann/json)

# Installation
## 1. Clone the repository:
```bash
git clone git@github.com:MelmanC/GameEngine.git
```

## 2. Run cmake to generate the build files and download most of the dependencies:
```bash
cmake -B .build && cmake --build .build
```

## 3. Run the editor:
```bash
./EditorApp
```

# Dependencies
- [raylib](https://www.raylib.com/) - A simple and easy-to-use library to create games.
- [raylib-cpp](https://github.com/RobLoach/raylib-cpp) - C++ wrapper for raylib.
- [nlohmann/json](https://github.com/nlohmann/json) - A JSON library for C++.
- [ImGui](https://github.com/ocornut/imgui) - A bloat-free graphical user interface library for C++.
- [rlImGui](https://github.com/raylib-extras/rlImGui) - A raylib binding for ImGui.

# Contributing
Contributions are welcome! Please read the [contributing guidelines](CONTRIBUTING.md)
before submitting a pull request.

# License
This project is licensed under the MIT License


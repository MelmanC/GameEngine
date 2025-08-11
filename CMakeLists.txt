cmake_minimum_required(VERSION 3.15)
project(GameEngine CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

execute_process(
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/compile_commands.json ${PROJECT_SOURCE_DIR}
)

add_compile_options(-W -Wall -Wextra -std=c++20)

include_directories(
    ${CMAKE_SOURCE_DIR}/Core
    ${CMAKE_SOURCE_DIR}/Editor
    ${CMAKE_SOURCE_DIR}/Editor/3D
    ${CMAKE_SOURCE_DIR}/Editor/Gui
    ${CMAKE_SOURCE_DIR}/Core/Camera
    ${CMAKE_SOURCE_DIR}/Core/Serialization
    ${CMAKE_SOURCE_DIR}/Core/Ecs
    ${CMAKE_SOURCE_DIR}/Core/Ecs/systems
    ${CMAKE_SOURCE_DIR}/Core/Ecs/components
    ${CMAKE_SOURCE_DIR}/Assets
)

find_package(raylib QUIET)
if(NOT raylib_FOUND)
    message(FATAL_ERROR "raylib not found")
endif()

find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.5.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

find_package(nlohmann_json 3.12.0 REQUIRED)
if(NOT nlohmann_json_FOUND)
    message(FATAL_ERROR "nlohmann_json not found")
endif()

FetchContent_Declare(imgui_external
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui_external)

FetchContent_Declare(rlimgui_external
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
)
FetchContent_MakeAvailable(rlimgui_external)

add_library(imgui
    ${imgui_external_SOURCE_DIR}/imgui.cpp
    ${imgui_external_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_external_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_external_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC ${imgui_external_SOURCE_DIR})

add_library(rlimgui
    ${rlimgui_external_SOURCE_DIR}/rlImGui.cpp
)
target_include_directories(rlimgui PUBLIC 
    ${rlimgui_external_SOURCE_DIR}
    ${imgui_external_SOURCE_DIR}
)
target_link_libraries(rlimgui PUBLIC imgui raylib)

add_library(EngineCore
    Core/Scene.cpp
    Core/Serialization/Load.cpp
    Core/Serialization/Save.cpp
    Core/Camera/Camera3D.cpp
    Core/Ecs/EntityManager.cpp
    Core/Ecs/systems/RenderSystem.cpp
    Core/Ecs/systems/SelectionSystem.cpp
    Core/Ecs/systems/GizmoSystem.cpp
)
target_link_libraries(EngineCore raylib raylib_cpp nlohmann_json::nlohmann_json imgui rlimgui)

add_executable(EditorApp
    Editor/main.cpp
    Editor/Window.cpp
    Editor/Gui/Gui.cpp
    Editor/InputManager.cpp
    Editor/Renderer.cpp
    Editor/ViewportManager.cpp
    Editor/Application.cpp
    Editor/3D/Shape.cpp
)
target_link_libraries(EditorApp raylib raylib_cpp nlohmann_json::nlohmann_json EngineCore imgui rlimgui)

# Runtime (commented out)
# add_executable(Runtime
#     Runtime/main.cpp
# )
# target_link_libraries(Runtime EngineCore raylib)

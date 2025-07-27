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
    ${CMAKE_SOURCE_DIR}/Assets
)

# include_directories(${CMAKE_SOURCE_DIR}/Core)
# include_directories(${CMAKE_SOURCE_DIR}/Assets)

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

add_library(EngineCore
    Core/GameShape.cpp
    Core/Scene.cpp
)
target_link_libraries(EngineCore raylib raylib_cpp nlohmann_json::nlohmann_json)

add_executable(EditorApp
    Editor/main.cpp
    Editor/Window.cpp
)
target_link_libraries(EditorApp raylib raylib_cpp nlohmann_json::nlohmann_json EngineCore)

# add_executable(Runtime
#     Runtime/main.cpp
# )
# target_link_libraries(Runtime EngineCore raylib)

if(UNIX AND NOT APPLE)
  target_link_libraries(EditorApp m pthread dl rt X11)
    target_link_libraries(Runtime m pthread dl rt X11)
endif()

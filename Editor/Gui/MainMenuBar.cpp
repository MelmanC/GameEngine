#include "MainMenuBar.hpp"
#include <iostream>
#include "Application.hpp"
#include "Gui.hpp"
#include "Shape.hpp"

void ui::MainMenuBar::draw(app::Application &app, ui::Gui &gui) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) {
        app.getScene().clearEntities();
        std::cout << "New scene created!" << std::endl;
      }
      if (ImGui::MenuItem("Open", "Ctrl+O")) {
        try {
          app.getScene().load("./scene.json");
          std::cout << "Scene loaded successfully!" << std::endl;
        } catch (const std::exception &e) {
          std::cerr << "Error loading scene: " << e.what() << std::endl;
        }
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        try {
          app.getScene().save("./scene.json");
          std::cout << "Scene saved successfully!" << std::endl;
        } catch (const std::exception &e) {
          std::cerr << "Error saving scene: " << e.what() << std::endl;
        }
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit")) {
        app.setRunning(false);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      ImGui::MenuItem("Hierarchy", nullptr, &gui._showHierarchy);
      ImGui::MenuItem("Properties", nullptr, &gui._showProperties);
      ImGui::MenuItem("Finder", nullptr, &gui._showFinder);
      ImGui::Separator();
      if (ImGui::BeginMenu("Panel Visibility")) {
        ImGui::MenuItem("Entity Info", nullptr, &gui._showEntities);
        ImGui::MenuItem("Transform Info", nullptr, &gui._showTransform);
        ImGui::MenuItem("Materials Info", nullptr, &gui._showMaterials);
        ImGui::MenuItem("Scripts Info", nullptr, &gui._showScripts);
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Create")) {
      shape3D::Shape shape;
      if (ImGui::MenuItem("Cube")) {
        shape.createCubeEntity({0.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f},
                               raylib::Color::White(), "New Cube",
                               &app.getECSManager());
      }
      if (ImGui::MenuItem("Sphere")) {
        shape.createSphereEntity({0.0f, 0.5f, 0.0f}, 1.0f,
                                 raylib::Color::White(), "New Sphere",
                                 &app.getECSManager());
      }
      if (ImGui::MenuItem("Cylinder")) {
        shape.createCylinderEntity({0.0f, 0.5f, 0.0f}, 1.0f, 2.0f,
                                   raylib::Color::White(), "New Cylinder",
                                   &app.getECSManager());
      }
      if (ImGui::MenuItem("Plane")) {
        shape.createPlaneEntity({0.0f, 0.0f, 0.0f}, {10.0f, 10.0f},
                                raylib::Color::White(), "New Plane",
                                &app.getECSManager());
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

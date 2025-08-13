#include "FinderPanel.hpp"
#include "Application.hpp"
#include "Shape.hpp"

void ui::FinderPanel::draw(app::Application &app, ui::Gui &gui) {
  if (!gui._showFinder)
    return;

  ImGui::SetNextWindowPos(ImVec2(250, app.getHeight() - 400),
                          ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(app.getWidth() - 550, 400),
                           ImGuiCond_FirstUseEver);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

  if (ImGui::Begin("Finder", &gui._showFinder, ImGuiWindowFlags_NoCollapse)) {
    ImGui::Columns(2, "finderColumns", true);
    ImGui::SetColumnWidth(0, 150);

    ImGui::Text("Dossiers :");
    std::string root = std::filesystem::current_path().string();

    for (const auto &entry : std::filesystem::directory_iterator(root)) {
      const std::string folderName = entry.path().filename().string();

      if (folderName[0] == '.')
        continue;

      bool selected = (_selectedDirectory == entry.path());

      if (entry.is_directory()) {
        const std::string folderName = entry.path().filename().string();
        ImGui::Image((ImTextureID)(intptr_t)gui._folderIcon.id, ImVec2(16, 16));
        ImGui::SameLine();

        if (ImGui::Selectable(folderName.c_str(), selected,
                              ImGuiSelectableFlags_None, ImVec2(0, 0))) {
          _selectedDirectory = entry.path();
        }
      }
    }

    ImGui::NextColumn();

    ImGui::Text("%s", _selectedDirectory.filename().string().c_str());
    ImGui::Separator();

    if (std::filesystem::exists(_selectedDirectory)) {
      for (const auto &entry :
           std::filesystem::directory_iterator(_selectedDirectory)) {
        if (_selectedDirectory == root)
          continue;
        const std::string name = entry.path().filename().string();
        if (name[0] == '.')
          continue;
        if (entry.is_directory()) {
          ImGui::Image((ImTextureID)(intptr_t)gui._folderIcon.id,
                       ImVec2(16, 16));
          ImGui::SameLine();
          if (ImGui::Selectable(name.c_str(), false, ImGuiSelectableFlags_None,
                                ImVec2(0, 0))) {
            _selectedDirectory = entry.path();
          }
        } else {
          ImGui::Image((ImTextureID)(intptr_t)gui._fileIcon.id, ImVec2(16, 16));
          ImGui::SameLine();
          if (ImGui::Selectable(name.c_str(), false, ImGuiSelectableFlags_None,
                                ImVec2(0, 0))) {
            shape3D::Shape shape;
            if (entry.path().extension() == ".gltf" ||
                entry.path().extension() == ".glb") {
              shape.createModelEntity(
                  {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, entry.path().string(),
                  raylib::Color::White(), name, &app.getECSManager());
            }
          }
        }
      }
    }
    ImGui::Columns(1);
  }

  ImGui::End();
  ImGui::PopStyleColor();
}

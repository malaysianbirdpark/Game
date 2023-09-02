#include "pch.h"
#include "D3D11ImGuiSceneBrowser.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneHolder.h"

#include <imgui.h>

std::optional<Engine::x_string> Engine::Graphics::D3D11ImGuiSceneBrowser::Browse() {
    x_string temp {};
    std::optional<x_string> result {};

    if (ImGui::Begin("Scene Browser")) {
        ImGui::SameLine();
        ImGui::SameLine(460);
        if (ImGui::Button("Submit") && !_selectedSceneTag.empty()) {
            temp = _selectedSceneTag;
            result.emplace(temp);

            _browserOpened = false;
            ImGui::End();
            return result;
        }

        ImGui::SameLine();
        _browserOpened = !ImGui::Button("Cancel");

        ImGuiShowSceneList();
    }
    ImGui::End();

    return result;
}

bool Engine::Graphics::D3D11ImGuiSceneBrowser::Opened() {
    return _browserOpened;
}

void Engine::Graphics::D3D11ImGuiSceneBrowser::SetOpened() {
    _browserOpened = true;
}

void Engine::Graphics::D3D11ImGuiSceneBrowser::ImGuiShowSceneList() {
    auto const& scene_map {D3D11SceneHolder::GetSceneMap()};

    static int pressed_count = 0;
    for (auto it {scene_map.begin()}; it != scene_map.end(); ++it)
    {
        auto const id {std::distance(scene_map.begin(), it)};

        // UV coordinates are often (0.0f, 0.0f) and (1.0f, 1.0f) to display an entire textures.
        // Here are trying to display only a 32x32 pixels area of the texture, hence the UV computation.
        // Read about UV coordinates here: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
        ImGui::PushID(id);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
        ImVec2 size = ImVec2(128.0f, 128.0f);                         // Size of the image we want to make visible
        ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
        ImVec2 uv1 = ImVec2(1.0f, 1.0f);    // UV coordinates for (32,32) in our texture
        ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
        if (ImGui::ImageButton("", 0, size, uv0, uv1, bg_col, tint_col))
            _selectedSceneTag = it->first;
        ImGui::SameLine();
        ImGui::Text(it->first.c_str());
        ImGui::PopStyleVar();
        ImGui::PopID();
        if (!(id & 0b1))
            ImGui::SameLine(300);
    }
}

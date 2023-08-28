#include "pch.h"
#include "D3D11ImGuiFileBrowser.h"

#include <imgui.h>

Engine::Graphics::D3D11ImGuiFileBrowser::D3D11ImGuiFileBrowser(char const* base_path) {
    std::filesystem::path path {base_path};
    CORE_ASSERT(exists(path), "File Path does not exist!");
    _basePath = path;
}

void Engine::Graphics::D3D11ImGuiFileBrowser::Print(std::filesystem::directory_iterator dit) {
    for (auto& content : dit) {
        std::cout << content.path().string() << std::endl;
        if (content.exists() && content.is_directory()) {
            std::filesystem::directory_iterator deep_dive {content.path()};
            Print(deep_dive);
        }
    }
}

void Engine::Graphics::D3D11ImGuiFileBrowser::TraverseDirectories(std::filesystem::directory_entry entry) {
    if (entry.exists()) {
        std::string const name {entry.path().has_filename() ? entry.path().filename().string() : entry.path().string()};
        std::string const label {name.empty() ? (std::string{"path"}) : name};
        
        int const flags {!entry.is_directory() ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet : 0};
        bool const opened {ImGui::TreeNodeEx(label.c_str(), flags)};

        ImGui::PushID(_id++);

        if (ImGui::IsItemClicked() && entry.is_regular_file()) {
            _selectedFilePath = entry.path().string();
            std::cout << "Selected File: " << label << std::endl;
        }

        if (opened && entry.is_directory()) {
            std::filesystem::directory_iterator const deep_dive {entry.path()};
            for (auto& dive : deep_dive)
                TraverseDirectories(dive);
        }

        if (opened)
            ImGui::TreePop();

        ImGui::PopID();
    }
}

std::optional<std::pair<int32_t, Engine::x_string>> Engine::Graphics::D3D11ImGuiFileBrowser::Browse() {
    std::filesystem::directory_entry const dit {_basePath};

    std::pair<int32_t, x_string> temp {};
    std::optional<std::pair<int32_t, x_string>> result {};

    _id = 0;
    if (ImGui::Begin("Asset Browser")) {
        ImGui::SameLine();
        static int selected_type = -1;
        const char* names[] = {
            "Emissive Map",
            "Diffuse Map",
            "Specular Map",
            "Normal Map", 
            "Height Map",
            "Metallic Map",
            "Roughness Map",
            "AmbientOcclusion Map",
            "BRDF Lut Map",
        };
        static bool toggles[] = { true, false, false, false, false, false };

        // Simple selection popup (if you want to show the current selection inside the Button itself,
        // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
        if (ImGui::Button("Select.."))
            ImGui::OpenPopup("my_select_popup");
        ImGui::SameLine();
        ImGui::TextUnformatted(selected_type == -1 ? "<None>" : names[selected_type]);
        if (ImGui::BeginPopup("my_select_popup"))
        {
            ImGui::Text("Texture Type");
            ImGui::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (ImGui::Selectable(names[i]))
                    selected_type = i;
            ImGui::EndPopup();
        }
        temp.first = selected_type;

        ImGui::SameLine(290);
        if (ImGui::Button("Submit") && !_selectedFilePath.empty()) {
            temp.second = _selectedFilePath;
            result.emplace(temp);

            _browserOpened = false;
            ImGui::End();
            return result;
        }

        ImGui::SameLine();
        _browserOpened = !ImGui::Button("Cancel");

        TraverseDirectories(dit);
    }
    ImGui::End();

    return result;
}

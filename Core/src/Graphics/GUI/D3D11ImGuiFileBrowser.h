#pragma once

#include <filesystem>

namespace Engine::Graphics {
    class D3D11ImGuiFileBrowser {
    public:
        D3D11ImGuiFileBrowser(char const* base_path);
        void Print(std::filesystem::directory_iterator dit);
        std::optional<std::pair<int32_t, x_string>> Browse();

        bool Opened() const { return _browserOpened; };
        void SetOpened() { _browserOpened = true; }
    private:
        void TraverseDirectories(std::filesystem::directory_entry entry);
    private:
        std::filesystem::path  _basePath;
        int32_t                _id {};
        bool                   _browserOpened {false};
        x_string               _selectedFilePath {};
    };
}


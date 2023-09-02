#pragma once

namespace Engine::Graphics {
    class D3D11ImGuiSceneBrowser {
    public:
        std::optional<x_string> Browse();
        bool Opened();
        void SetOpened();
    private:
        void ImGuiShowSceneList();
    private:
        bool                   _browserOpened {false};
        x_string               _selectedSceneTag {""};
    };
}


#pragma once

namespace Engine::Graphics {
    class AssimpLoader {
    public:
        enum RenderAPI {
            D3D11,            
            D3D12,
            OPENGL,
            VULKAN
        };

        static void LoadAll(RenderAPI api = D3D11);
        static void LoadFile(char const* path, RenderAPI = D3D11);
    private:
        static void SaveSceneForD3D11();
    private:
        inline static x_unordered_map<RenderAPI, std::function<void(void)>> _apiMap {
            {D3D11, &AssimpLoader::SaveSceneForD3D11}
        };
    };
}


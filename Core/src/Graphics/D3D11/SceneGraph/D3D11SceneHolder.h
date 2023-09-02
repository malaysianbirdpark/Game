#pragma once

#include <bitset>
#include <imgui.h>

#include <assimp/scene.h>

#include "D3D11SceneGraph.h"

namespace Engine::Graphics {
    class D3D11Mesh;
    class D3D11Material;
    class D3D11VertexBuffer;
    class D3D11IndexBuffer;

    class D3D11SceneHolder {
    public:
        static void Load(ID3D11Device& device, ID3D11DeviceContext& context);
        static D3D11SceneGraph ResolveScene(x_string const& tag);
        static bool Contains(x_string const& tag);
        static x_unordered_map<x_string, std::unique_ptr<D3D11SceneGraph>> const& GetSceneMap();
        static x_vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> const& GetPreviewImages();
    private:
        inline static x_unordered_map<x_string, std::unique_ptr<D3D11SceneGraph>> _scenes;
        inline static x_vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> _previewImages;
    };
}


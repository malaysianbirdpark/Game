#pragma once

#include <bitset>

#include <assimp/scene.h>

namespace Engine::Graphics {
    class D3D11SceneGraph;
    class D3D11Mesh;
    class D3D11Material;
    class D3D11VertexBuffer;
    class D3D11IndexBuffer;

    class D3D11SceneHolder {
    public:
        static void Load(ID3D11Device& device);
        static std::shared_ptr<D3D11SceneGraph> ResolveScene(x_string const& tag);
    private:
    private:
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11SceneGraph>> _scenes;
    };
}


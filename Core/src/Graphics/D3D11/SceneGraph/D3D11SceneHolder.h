#pragma once

#include <bitset>

#include <assimp/scene.h>

#include "D3D11SceneGraph.h"

namespace Engine::Graphics {
    class D3D11Mesh;
    class D3D11Material;
    class D3D11VertexBuffer;
    class D3D11IndexBuffer;

    class D3D11SceneHolder {
    public:
        static void Load(ID3D11Device& device);
        static D3D11SceneGraph ResolveScene(x_string const& tag);
    private:
        inline static x_unordered_map<x_string, std::unique_ptr<D3D11SceneGraph>> _scenes;
    };
}


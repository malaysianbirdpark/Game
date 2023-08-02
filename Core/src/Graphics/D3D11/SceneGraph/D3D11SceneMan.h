#pragma once

#include <bitset>

namespace Engine::Graphics {
    class D3D11SceneGraph;

    class D3D11SceneMan {
    public:
        static void Load(ID3D11Device& device);
        static std::shared_ptr<D3D11SceneGraph> ResolveScene(x_string const& tag);
    private:
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11SceneGraph>> _scenes;
    };
}


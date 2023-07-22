#pragma once

namespace Engine::Graphics {
    class D3D11RootSignature;

    class D3D11RSLibrary {
    public:
        static void RegisterRS(x_string const& tag, std::shared_ptr<D3D11RootSignature> rs);
        static std::shared_ptr<D3D11RootSignature> ResolveRS(x_string const& tag);
    private:
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11RootSignature>> _lib;
    };
}


#pragma once

namespace Engine::Graphics {
    class D3D11PipelineStateObject;

    class D3D11PSOLibrary {
    public:
        static void Init(ID3D11Device& device);
        static void RegisterPSO(x_string const& tag, std::shared_ptr<D3D11PipelineStateObject> const& pso);
        static std::shared_ptr<D3D11PipelineStateObject> ResolvePSO(x_string const& tag);
    private:
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11PipelineStateObject>> _lib;
    };
}


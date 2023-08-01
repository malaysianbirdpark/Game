#pragma once

#include "ShaderResource/D3D11ShaderResource.h"

namespace Engine::Graphics {
    class D3D11Sampler;

    class D3D11RootSignatureHolder {
    public:
        static std::shared_ptr<D3D11Sampler>           ResolveSampler(ID3D11Device& device, char const* tag);
        static std::shared_ptr<D3D11ShaderResource>    ResolveShaderResource(ID3D11Device& device, char const* path);
    private:
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11Sampler>>            _samplers;
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11ShaderResource>>     _srs;
    };
}

#pragma once

namespace Engine::Graphics {
    class D3D11ConstantBuffer;
    class D3D11Sampler;
    class D3D11Texture;

    class D3D11RootSignatureHolder {
    public:
        D3D11RootSignatureHolder() = default;
        ~D3D11RootSignatureHolder() = default;

        static std::shared_ptr<D3D11ConstantBuffer>    ResolveConstantBuffer(ID3D11Device& device, void* data, size_t data_size, bool vsVisible, bool psVisible, bool dynamic, char const* tag);
        static std::shared_ptr<D3D11Sampler>           ResolveSampler(ID3D11Device& device, char const* tag);
        static std::shared_ptr<D3D11Texture>           ResolveTexture(ID3D11Device& device, char const* path);
    private:
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11ConstantBuffer>>     _cbs;
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11Sampler>>            _samplers;
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11Texture>>            _textures;
    };
}

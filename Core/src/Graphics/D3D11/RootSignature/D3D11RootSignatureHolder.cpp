#include "pch.h"
#include "D3D11RootSignatureHolder.h"

#include "D3D11RootSignatureCommon.h"

std::shared_ptr<Engine::Graphics::D3D11ConstantBuffer> Engine::Graphics::D3D11RootSignatureHolder::
ResolveConstantBuffer(ID3D11Device& device, void* data, size_t data_size, bool vsVisible, bool psVisible, bool dynamic, char const* tag) {
    auto const uid {D3D11ConstantBuffer::GenUID(vsVisible, psVisible, dynamic, tag)};

    if (_cbs.contains(uid))
        return _cbs[uid];
    return _cbs[uid] = MakeShared<D3D11ConstantBuffer>(device, data, data_size, vsVisible, psVisible, dynamic, tag);
}

std::shared_ptr<Engine::Graphics::D3D11Sampler> Engine::Graphics::D3D11RootSignatureHolder::ResolveSampler(ID3D11Device& device, char const* tag) {
    auto const uid {D3D11Sampler::GenUID(tag)};

    if (_samplers.contains(uid))
        return _samplers[uid];
    return _samplers[uid] = MakeShared<D3D11Sampler>(device, tag);
}

std::shared_ptr<Engine::Graphics::D3D11Texture> Engine::Graphics::D3D11RootSignatureHolder::ResolveTexture(ID3D11Device& device, char const* path) {
    auto const uid {D3D11Texture::GenUID(path)};

    if (_textures.contains(uid))
        return _textures[uid];
    return _textures[uid] = MakeShared<D3D11Texture>(device, path);
}

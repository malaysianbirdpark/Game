#include "pch.h"
#include "D3D11RootSignatureHolder.h"

#include "D3D11RootSignatureCommon.h"

std::shared_ptr<Engine::Graphics::D3D11ConstantBuffer> Engine::Graphics::D3D11RootSignatureHolder::
ResolveConstantBuffer(ID3D11Device& device, char const* tag) {
    auto const uid {std::visit(GenUIDConstantBuffer{tag}, )};

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

std::shared_ptr<Engine::Graphics::D3D11ShaderResource> Engine::Graphics::D3D11RootSignatureHolder::ResolveShaderResource(ID3D11Device& device, char const* path) {
    auto const uid {D3D11ShaderResource::GenUID(path)};

    if (_srs.contains(uid))
        return _srs[uid];
    return _srs[uid] = MakeShared<D3D11ShaderResource>(device, path);
}

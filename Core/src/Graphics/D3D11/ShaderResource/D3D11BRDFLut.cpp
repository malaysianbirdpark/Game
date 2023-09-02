#include "pch.h"
#include "D3D11BRDFlut.h"

#include <directxtk/DDSTextureLoader.h>

Engine::Graphics::D3D11BRDFLut::D3D11BRDFLut(ID3D11Device& device, char const* path)
    : _path{path}
{
    using namespace DirectX;

    std::wstring p (_path.length(), L' ');
    std::ranges::copy(_path, p.begin());

    auto alphaMode {DDS_ALPHA_MODE_STRAIGHT};

    CreateDDSTextureFromFileEx(
        &device,
        p.c_str(),
        0,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
        D3D11_CPU_ACCESS_READ,
        0u,
        DDS_LOADER_DEFAULT,
        nullptr,
        _srv.ReleaseAndGetAddressOf(),
        &alphaMode
    );
}

std::shared_ptr<Engine::Graphics::D3D11BRDFLut> Engine::Graphics::D3D11BRDFLut::Create(ID3D11Device& device, char const* path) {
    return std::move(MakeShared<D3D11BRDFLut>(device, path));
}

void Engine::Graphics::D3D11BRDFLut::Bind(ID3D11DeviceContext& context) const {
    context.PSSetShaderResources(8u, 1u, _srv.GetAddressOf());
}

Engine::x_string Engine::Graphics::D3D11BRDFLut::GetDescription() const {
    return {"BRDF Lookup Table"};
}

int32_t Engine::Graphics::D3D11BRDFLut::GetTypeID() const {
    return 8;
}

Engine::x_string Engine::Graphics::D3D11BRDFLut::GenUID(char const* path) {
    return x_string{typeid(D3D11BRDFLut).name()} + "#" + path;
}

Engine::x_string Engine::Graphics::D3D11BRDFLut::GetUID() const {
    return GenUID(_path.c_str());
}


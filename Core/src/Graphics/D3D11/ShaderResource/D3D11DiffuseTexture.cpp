#include "pch.h"
#include "D3D11DiffuseTexture.h"

#include <directxtk/DDSTextureLoader.h>

Engine::Graphics::D3D11DiffuseTexture::D3D11DiffuseTexture(ID3D11Device& device, char const* path)
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
        D3D11_RESOURCE_MISC_GENERATE_MIPS,
        DDS_LOADER_DEFAULT,
        nullptr,
        _srv.ReleaseAndGetAddressOf(),
        &alphaMode
    );
}

void Engine::Graphics::D3D11DiffuseTexture::Bind(ID3D11DeviceContext& context) const {
    context.PSSetShaderResources(0u, 1u, _srv.GetAddressOf());
}

Engine::x_string Engine::Graphics::D3D11DiffuseTexture::GenUID(char const* path) {
    return x_string{typeid(D3D11DiffuseTexture).name()} + "#" + path;
}

Engine::x_string Engine::Graphics::D3D11DiffuseTexture::GetUID() const {
    return GenUID(_path.c_str());
}
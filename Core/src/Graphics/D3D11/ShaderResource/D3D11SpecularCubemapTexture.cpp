#include "pch.h"
#include "D3D11SpecularCubemapTexture.h"

#include <directxtk/DDSTextureLoader.h>

Engine::Graphics::D3D11SpecularCubemapTexture::D3D11SpecularCubemapTexture(ID3D11Device& device, char const* path)
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
        D3D11_BIND_SHADER_RESOURCE, 
        0u,
        D3D11_RESOURCE_MISC_TEXTURECUBE,
        DDS_LOADER_DEFAULT,
        nullptr,
        _srv.ReleaseAndGetAddressOf(),
        &alphaMode
    );

}

std::shared_ptr<Engine::Graphics::D3D11SpecularCubemapTexture> Engine::Graphics::D3D11SpecularCubemapTexture::CreateSpecularCubemapTexture(ID3D11Device& device, char const* path) {
    return std::move(MakeShared<D3D11SpecularCubemapTexture>(device, path));
}

void Engine::Graphics::D3D11SpecularCubemapTexture::Bind(ID3D11DeviceContext& context) const {
    context.PSSetShaderResources(7u, 1u, _srv.GetAddressOf());
}

Engine::x_string Engine::Graphics::D3D11SpecularCubemapTexture::GenUID(char const* path) {
    return x_string{typeid(D3D11SpecularCubemapTexture).name()} + "#" + path;
}

Engine::x_string Engine::Graphics::D3D11SpecularCubemapTexture::GetUID() const {
    return GenUID(_path.c_str());
}


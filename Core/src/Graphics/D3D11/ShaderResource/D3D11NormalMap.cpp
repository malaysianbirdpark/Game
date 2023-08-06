#include "pch.h"
#include "D3D11NormalMap.h"

#include <directxtk/DDSTextureLoader.h>

Engine::Graphics::D3D11NormalMap::D3D11NormalMap(ID3D11Device& device, char const* path)
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

Engine::Graphics::D3D11NormalMap Engine::Graphics::D3D11NormalMap::CreateNormalMap(ID3D11Device& device, char const* path) {
    return D3D11NormalMap{device, path};
}

void Engine::Graphics::D3D11NormalMap::Bind(ID3D11DeviceContext& context) const {
    context.PSSetShaderResources(1u, 1u, _srv.GetAddressOf());
}

Engine::x_string Engine::Graphics::D3D11NormalMap::GenUID(char const* path) {
    return x_string{typeid(D3D11NormalMap).name()} + "#" + path;
}

Engine::x_string Engine::Graphics::D3D11NormalMap::GetUID() const {
    return GenUID(_path.c_str());
}

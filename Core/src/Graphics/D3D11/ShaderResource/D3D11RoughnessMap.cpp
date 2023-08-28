#include "pch.h"
#include "D3D11RoughnessMap.h"

#include <directxtk/DDSTextureLoader.h>

Engine::Graphics::D3D11RoughnessMap::D3D11RoughnessMap(ID3D11Device& device, char const* path)
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

std::shared_ptr<Engine::Graphics::D3D11RoughnessMap> Engine::Graphics::D3D11RoughnessMap::Create(ID3D11Device& device, char const* path) {
    return std::move(MakeShared<D3D11RoughnessMap>(device, path));
}

void Engine::Graphics::D3D11RoughnessMap::Bind(ID3D11DeviceContext& context) const {
    context.PSSetShaderResources(6u, 1u, _srv.GetAddressOf());
}

Engine::x_string Engine::Graphics::D3D11RoughnessMap::GetDescription() const {
    return {"Roughness Map"};
}

int32_t Engine::Graphics::D3D11RoughnessMap::GetTypeID() const {
    return 6;
}

Engine::x_string Engine::Graphics::D3D11RoughnessMap::GenUID(char const* path) {
    return x_string{typeid(D3D11RoughnessMap).name()} + "#" + path;
}

Engine::x_string Engine::Graphics::D3D11RoughnessMap::GetUID() const {
    return GenUID(_path.c_str());
}


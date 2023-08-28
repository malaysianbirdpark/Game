#include "pch.h"
#include "D3D11GlossinessMap.h"

#include <directxtk/DDSTextureLoader.h>

Engine::Graphics::D3D11GlossinessMap::D3D11GlossinessMap(ID3D11Device& device, char const* path)
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

std::shared_ptr<Engine::Graphics::D3D11GlossinessMap> Engine::Graphics::D3D11GlossinessMap::Create(ID3D11Device& device, char const* path) {
    return std::move(MakeShared<D3D11GlossinessMap>(device, path));
}

void Engine::Graphics::D3D11GlossinessMap::Bind(ID3D11DeviceContext& context) const {
    context.PSSetShaderResources(9u, 1u, _srv.GetAddressOf());
}

Engine::x_string Engine::Graphics::D3D11GlossinessMap::GetDescription() const {
    return {"Glossiness Map"};
}

int32_t Engine::Graphics::D3D11GlossinessMap::GetTypeID() const {
    return 9;
}

Engine::x_string Engine::Graphics::D3D11GlossinessMap::GenUID(char const* path) {
    return x_string{typeid(D3D11GlossinessMap).name()} + "#" + path;
}

Engine::x_string Engine::Graphics::D3D11GlossinessMap::GetUID() const {
    return GenUID(_path.c_str());
}


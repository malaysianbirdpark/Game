#include "pch.h"
#include "D3D11OpacityMap.h"

#include <directxtk/DDSTextureLoader.h>

Engine::Graphics::D3D11OpacityMap::D3D11OpacityMap(ID3D11Device& device, ID3D11DeviceContext& context, char const* path)
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
    context.GenerateMips(_srv.Get());
}

std::shared_ptr<Engine::Graphics::D3D11OpacityMap> Engine::Graphics::D3D11OpacityMap::Create(ID3D11Device& device, ID3D11DeviceContext& context, char const* path) {
    return std::move(MakeShared<D3D11OpacityMap>(device, context, path));
}

void Engine::Graphics::D3D11OpacityMap::Bind(ID3D11DeviceContext& context) const {
    context.PSSetShaderResources(5u, 1u, _srv.GetAddressOf());
}

Engine::x_string Engine::Graphics::D3D11OpacityMap::GetDescription() const {
    return {"Opacity Map"};
}

int32_t Engine::Graphics::D3D11OpacityMap::GetTypeID() const {
    return 5;
}

Engine::x_string Engine::Graphics::D3D11OpacityMap::GenUID(char const* path) {
    return x_string{typeid(D3D11OpacityMap).name()} + "#" + path;
}

Engine::x_string Engine::Graphics::D3D11OpacityMap::GetUID() const {
    return GenUID(_path.c_str());
}


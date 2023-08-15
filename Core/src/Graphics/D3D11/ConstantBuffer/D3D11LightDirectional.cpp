#include "pch.h"
#include "D3D11LightDirectional.h"

#include <imgui.h>

Engine::Graphics::D3D11LightDirectional::D3D11LightDirectional(ID3D11Device& device, DirectX::XMFLOAT3&& dir, DirectX::XMFLOAT3&& color)
    : _info{._diffuseColor = std::move(color), ._dir = std::move(dir)}
{
    D3D11_BUFFER_DESC bd {};
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(LightInfo); 
    bd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA sd {};
    sd.pSysMem = &_info;

    device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11LightDirectional::ImGuiShowEditWindow() {
    if (ImGui::Begin("Directional Light")) {
        ImGui::Text("Color");
        ImGui::ColorPicker3("Color", &_info._diffuseColor.x);

        ImGui::Text("Direction");
        ImGui::SliderFloat3("XYZ", &_info._dir.x, -1.0f, 1.0f);
    }
    ImGui::End();
}

void Engine::Graphics::D3D11LightDirectional::Update() {
    ImGuiShowEditWindow();
}

void Engine::Graphics::D3D11LightDirectional::Upload(ID3D11DeviceContext& context) {
    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &_info, sizeof(LightInfo));
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11LightDirectional::Bind(ID3D11DeviceContext& context) {
    Upload(context);
    context.PSSetConstantBuffers(1u, 1u, _resource.GetAddressOf());
}

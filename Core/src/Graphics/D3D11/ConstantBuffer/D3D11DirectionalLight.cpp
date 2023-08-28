#include "pch.h"
#include "D3D11DirectionalLight.h"

#include <imgui.h>

Engine::Graphics::D3D11DirectionalLight::D3D11DirectionalLight(ID3D11Device& device, DirectX::XMFLOAT3&& dir, DirectX::XMFLOAT3&& color)
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

void Engine::Graphics::D3D11DirectionalLight::Update(float const dt) {
    _interacted = 0;
    if (ImGui::Begin("Directional Light")) {
        ImGui::Text("Color");
        _interacted += ImGui::ColorEdit3("Color", &_info._diffuseColor.x);

        ImGui::Text("Direction");
        _interacted += ImGui::SliderFloat3("XYZ", &_info._dir.x, -1.0f, 1.0f);
    }
    ImGui::End();
}

void Engine::Graphics::D3D11DirectionalLight::Upload(ID3D11DeviceContext& context) {
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

void Engine::Graphics::D3D11DirectionalLight::Bind(ID3D11DeviceContext& context) {
    if (_interacted != 0)
        Upload(context);
    context.PSSetConstantBuffers(4u, 1u, _resource.GetAddressOf());
}

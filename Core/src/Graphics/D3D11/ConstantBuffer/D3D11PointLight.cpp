#include "pch.h"
#include "D3D11PointLight.h"


#include <imgui.h>

#include "Graphics/D3D11/D3D11Core.h"
#include "Graphics/D3DCamera.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneHolder.h"

Engine::Graphics::D3D11PointLight::D3D11PointLight(ID3D11Device& device)
    : _info{}
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

void Engine::Graphics::D3D11PointLight::Update(float const dt) {
    _interacted = 0;
    ImGui::Text("Color");
    _interacted += ImGui::ColorEdit3("Color", &_info._color.x);
    ImGui::Text("Range");
    _interacted += ImGui::SliderFloat("Range", &_range, 0.0001f, 300.0f);
    _info._reciprocalRange = 1.0f / _range;
}

void Engine::Graphics::D3D11PointLight::SetPosition(DirectX::XMFLOAT3 pos) {
    _interacted = 1;
    _info._pos.x = pos.x;
    _info._pos.y = pos.y;
    _info._pos.z = pos.z;
}

void Engine::Graphics::D3D11PointLight::Upload(ID3D11DeviceContext& context) {
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

void Engine::Graphics::D3D11PointLight::Bind(ID3D11DeviceContext& context) {
    if (_interacted != 0)
        Upload(context);
    context.PSSetConstantBuffers(5u, 1u, _resource.GetAddressOf());
}


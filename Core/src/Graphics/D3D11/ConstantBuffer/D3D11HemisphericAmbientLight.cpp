#include "pch.h"
#include "D3D11HemisphericAmbientLight.h"

#include <imgui.h>

Engine::Graphics::D3D11HemisphericAmbientLight::D3D11HemisphericAmbientLight(ID3D11Device& device)
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

void Engine::Graphics::D3D11HemisphericAmbientLight::Update(float const dt) {
    _interacted = 0;
    if (ImGui::Begin("Hemispheric Ambient Light")) {
        ImGui::Text("Down Color");
        _interacted += ImGui::ColorEdit3("Down Color", &_info._ambientDownColor.x);
        ImGui::Text("Up Color");
        _interacted += ImGui::ColorEdit3("Up Color", &_info._ambientUpColor.x);
    }
    ImGui::End();
}

void Engine::Graphics::D3D11HemisphericAmbientLight::Upload(ID3D11DeviceContext& context) {
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

void Engine::Graphics::D3D11HemisphericAmbientLight::Bind(ID3D11DeviceContext& context) {
    if (_interacted != 0)
        Upload(context);
    context.PSSetConstantBuffers(3u, 1u, _resource.GetAddressOf());
}


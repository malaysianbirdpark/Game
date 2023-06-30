#include "pch.h"
#include "D3D11ConstantBuffer.h"

Engine::Graphics::D3D11ConstantBuffer::D3D11ConstantBuffer(ID3D11Device& device, void const* data, size_t data_size, bool vertex_shader_visible, bool pixel_shader_visible, bool dynamic, char const* tag)
    : _tag{tag}, _vsVisible{vertex_shader_visible}, _psVisible{pixel_shader_visible}, _dynamic{dynamic}
{
        D3D11_BUFFER_DESC bd {};
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = _dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
        bd.CPUAccessFlags = _dynamic? D3D11_CPU_ACCESS_WRITE : D3D11_CPU_ACCESS_READ;
        bd.MiscFlags = 0;
        bd.ByteWidth = data_size; 
        bd.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA sd {};
        sd.pSysMem = data;

        device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11ConstantBuffer::Update(ID3D11DeviceContext& context, void const* data, size_t data_size) {
    if (!_dynamic)
        return;

    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &data, data_size);
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11ConstantBuffer::Bind(ID3D11DeviceContext& context, UINT slot) {
    if (_vsVisible) [[likely]]
        context.VSSetConstantBuffers(slot, 1u, _resource.GetAddressOf());
    if (_psVisible)
        context.PSSetConstantBuffers(slot, 1u, _resource.GetAddressOf());
}

Engine::x_string Engine::Graphics::D3D11ConstantBuffer::GenUID(bool vsVisible, bool psVisible, bool dynamic, char const* tag) {
    return x_string{typeid(D3D11ConstantBuffer).name()} + "#" + tag + (vsVisible ? "1" : "0") + (psVisible ? "1" : "0") + (dynamic ? "1" : "0");
}

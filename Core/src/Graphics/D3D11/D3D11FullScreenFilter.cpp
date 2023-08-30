#include "pch.h"
#include "D3D11FullScreenFilter.h"

#include "PipelineState/D3D11PSOLibrary.h"
#include "PipelineState/D3D11PipelineStateObject.h"

Engine::Graphics::D3D11FullScreenFilter::D3D11FullScreenFilter(ID3D11Device& device)
    : _vertex{device, sizeof(FullScreenVertex), sizeof(FullScreenVertex) * _vertices.size(), _vertices.data(), "fullscreen"},
      _index{device, _indices.data(), _indices.size(), "fullscreen"}
{
    _psoBlur = D3D11PSOLibrary::ResolvePSO("fullscreen_blur");

    D3D11_SAMPLER_DESC sd {};
    sd.Filter = D3D11_FILTER_ANISOTROPIC;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
    sd.MipLODBias = 0.0f;
    sd.MinLOD = 0.0f;
    sd.MaxLOD = D3D11_FLOAT32_MAX;

    device.CreateSamplerState(&sd, _sampler.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11FullScreenFilter::Render(ID3D11DeviceContext& context, ID3D11ShaderResourceView* const* input, ID3D11RenderTargetView* const* output) {
    context.IASetPrimitiveTopology(_topology);
    _vertex.Bind(context);
    _index.Bind(context);

    context.PSSetSamplers(1u, 1u, _sampler.GetAddressOf());
    _psoBlur->Bind(context);

    context.OMSetRenderTargets(1u, output, nullptr);
    context.PSSetShaderResources(23u, 1u, input);
    context.DrawIndexed(_index.GetCount(), 0u, 0u);
}

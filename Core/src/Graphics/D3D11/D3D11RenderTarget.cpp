#include "pch.h"
#include "D3D11RenderTarget.h"

Engine::Graphics::D3D11RenderTarget::D3D11RenderTarget(ID3D11Device& device, D3D11_TEXTURE2D_DESC const& texture_desc) {
    device.CreateTexture2D(&texture_desc, nullptr, _buffer.ReleaseAndGetAddressOf());
    device.CreateShaderResourceView(_buffer.Get(), nullptr, _srv.ReleaseAndGetAddressOf());
    device.CreateRenderTargetView(_buffer.Get(), nullptr, _rtv.ReleaseAndGetAddressOf());
}

ID3D11ShaderResourceView* Engine::Graphics::D3D11RenderTarget::GetSRV() {
    return _srv.Get();
}

void Engine::Graphics::D3D11RenderTarget::SetRTV(ID3D11DeviceContext& context, ID3D11DepthStencilView* dsv) {
    context.OMSetRenderTargets(1u, _rtv.GetAddressOf(), dsv);
}

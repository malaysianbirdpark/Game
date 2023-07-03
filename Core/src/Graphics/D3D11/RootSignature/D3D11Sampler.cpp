#include "pch.h"
#include "D3D11Sampler.h"

namespace Engine::Graphics {
    D3D11Sampler::D3D11Sampler(ID3D11Device& device, char const* tag)
        : _tag{tag}
    {
        D3D11_SAMPLER_DESC sd {};
        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        device.CreateSamplerState(&sd, _sampler.ReleaseAndGetAddressOf());
    }

    void D3D11Sampler::Bind(ID3D11DeviceContext& context, UINT slot) noexcept {
        context.PSSetSamplers(slot, 1u, _sampler.GetAddressOf());
    }

    x_string D3D11Sampler::GenUID(char const* tag) {
        return x_string{typeid(D3D11Sampler).name()} + "#" + tag;
    }

    x_string D3D11Sampler::GetUID() const {
        return GenUID(_tag.c_str());
    }
}
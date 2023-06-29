#include "gwpch.h"
#include "D3D11Sampler.h"

#include "BindableCodex.h"

#include "Renderer/RenderDevice.h"

namespace Glowing::Bind {
    Sampler::Sampler() {
        D3D11_SAMPLER_DESC sd {};
        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        GET_DEVICE->CreateSamplerState(&sd, pSampler.ReleaseAndGetAddressOf());
    }

    void Sampler::Bind() noexcept {
        GET_CONTEXT->PSSetSamplers(0u, 1u, pSampler.GetAddressOf());
    }

    std::shared_ptr<Sampler> Sampler::Resolve() {
        return Codex::Resolve<Sampler>();
    }

    std::string Sampler::GenerateUID() {
        return typeid(Sampler).name();
    }

    std::string Sampler::GetUID() const noexcept {
        return GenerateUID();
    }
}

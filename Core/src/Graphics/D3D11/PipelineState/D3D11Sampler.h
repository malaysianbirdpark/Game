#pragma once

#include "D3D11Bindable.h"

namespace Glowing::Bind {
    class GLOWING_API Sampler final : public Bindable {
    public:
        Sampler();
        void Bind() noexcept override;

        [[nodiscard]] static std::shared_ptr<Sampler> Resolve();
        [[nodiscard]] static std::string GenerateUID();
        [[nodiscard]] std::string GetUID() const noexcept override;
    protected:
        Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
    };
}


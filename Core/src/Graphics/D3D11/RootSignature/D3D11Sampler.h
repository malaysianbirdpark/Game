#pragma once

namespace Engine::Graphics {
    class D3D11Sampler {
    public:
        D3D11Sampler(ID3D11Device& device, char const* tag);

        void Bind(ID3D11DeviceContext& context, UINT slot) noexcept;

        [[nodiscard]] static x_string GenUID(char const* tag);
        [[nodiscard]] x_string GetUID() const;
    protected:
        x_string _tag;
        Microsoft::WRL::ComPtr<ID3D11SamplerState> _sampler;
    };
}


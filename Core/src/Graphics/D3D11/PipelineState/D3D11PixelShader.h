#pragma once

namespace Engine::Graphics {
    class D3D11PixelShader {
    public:
        D3D11PixelShader(ID3D11Device& device, char const* path);

        void Bind(ID3D11DeviceContext& context) noexcept;
        [[nodiscard]] static x_string GenUID(x_string const& path);
        [[nodiscard]] x_string GetUID() const;
    protected:
        x_string _path;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    };
}


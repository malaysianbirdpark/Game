#pragma once

namespace Engine::Graphics {
    class D3D11Texture {
    public:
        D3D11Texture(ID3D11Device& device, char const* path);

        void Bind(ID3D11DeviceContext& context, UINT slot) noexcept;

        [[nodiscard]] static x_string GenUID(char const* path);
        [[nodiscard]] x_string GetUID() const;
    protected:
        x_string _path;
        x_vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> _textureView;
    };
}


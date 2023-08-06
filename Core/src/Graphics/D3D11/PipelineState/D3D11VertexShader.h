#pragma once

namespace Engine::Graphics {
    class D3D11VertexShader {
    public:
        D3D11VertexShader(ID3D11Device& device, char const* path);

        void Bind(ID3D11DeviceContext& context) noexcept;
        [[nodiscard]] ID3DBlob* GetByteCode() const noexcept;
        [[nodiscard]] static x_string GenUID(x_string const& path);
        [[nodiscard]] x_string GetUID() const;
    protected:
        x_string _path;
        Microsoft::WRL::ComPtr<ID3DBlob> _byteCode;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
    };
}


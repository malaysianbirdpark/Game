#pragma once

namespace Engine::Graphics {
    class D3D11VertexBuffer {
    public:
        D3D11VertexBuffer(ID3D11Device& device, UINT stride, UINT byte_width, void const* data, char const* tag);

        void Bind(ID3D11DeviceContext& context) noexcept;
        [[nodiscard]] static x_string GenUID(char const* tag);
        [[nodiscard]] x_string GetUID() const;
    private:
        x_string _tag;
        UINT _stride;
        Microsoft::WRL::ComPtr<ID3D11Buffer> _VertexBuffer;
    };
}


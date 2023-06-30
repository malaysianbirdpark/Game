#pragma once

namespace Engine::Graphics {
    namespace Vertex {
        class Buffer;
    }

    class D3D11VertexBuffer {
    public:
        D3D11VertexBuffer(ID3D11Device& device, Vertex::Buffer const& vbuf, char const* tag);

        void Bind(ID3D11DeviceContext& context) noexcept;
        [[nodiscard]] static x_string GenUID(char const* tag);
        [[nodiscard]] x_string GetUID() const;
    private:
        x_string _tag;
        UINT stride;
        Microsoft::WRL::ComPtr<ID3D11Buffer> p_VertexBuffer;
    };
}


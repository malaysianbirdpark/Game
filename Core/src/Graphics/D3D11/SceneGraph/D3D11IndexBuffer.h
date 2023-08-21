#pragma once

namespace Engine::Graphics {
    class D3D11IndexBuffer {
    public:
        D3D11IndexBuffer(ID3D11Device& device, void const* data, UINT count, char const* tag);

        void Bind(ID3D11DeviceContext& context) noexcept;
        [[nodiscard]] UINT GetCount() const noexcept;
        [[nodiscard]] static x_string GenUID(char const* tag);
        [[nodiscard]] x_string GetUID() const;
    protected:
        x_string _tag;
        UINT _count;
        Microsoft::WRL::ComPtr<ID3D11Buffer> _indexBuffer;
    };
}


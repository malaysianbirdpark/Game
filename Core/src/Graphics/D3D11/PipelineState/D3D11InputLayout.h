#pragma once

#include "..\..\D3DVertex.h"

namespace Engine::Graphics {
    class D3D11InputLayout {
    public:
        D3D11InputLayout(ID3D11Device& device, Vertex::Layout const& layout, ID3DBlob* p_vertex_shader_byte_code);

        void Bind(ID3D11DeviceContext& context) noexcept;
        [[nodiscard]] static x_string GenUID(Vertex::Layout const& layout);
        [[nodiscard]] x_string GetUID() const;
    private:
        Vertex::Layout _layout;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
    };
}


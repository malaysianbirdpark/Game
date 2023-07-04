#include "pch.h"
#include "D3D11InputLayout.h"

namespace Engine::Graphics {
    D3D11InputLayout::D3D11InputLayout(ID3D11Device& device, Vertex::Layout const& layout, ID3DBlob* p_vertex_shader_byte_code)
        : _layout{layout}
    {
        auto const D3DLayout {layout.GetD3D11Layout()};
        device.CreateInputLayout(
            D3DLayout.data(),
            static_cast<UINT>(D3DLayout.size()),
            p_vertex_shader_byte_code->GetBufferPointer(),
            p_vertex_shader_byte_code->GetBufferSize(),
            _inputLayout.ReleaseAndGetAddressOf()
        );
    }

    void D3D11InputLayout::Bind(ID3D11DeviceContext& context) noexcept {
        context.IASetInputLayout(_inputLayout.Get());
    }

    x_string D3D11InputLayout::GenUID(Vertex::Layout const& layout) {
        return x_string{typeid(D3D11InputLayout).name()} + "#" + layout.GetCode();
    }

    x_string D3D11InputLayout::GetUID() const {
        return GenUID(_layout);
    }
}

#include "pch.h"
#include "D3D11InputLayout.h"

namespace Engine::Graphics {
    D3D11InputLayout::D3D11InputLayout(ID3D11Device& device, x_vector<D3D11_INPUT_ELEMENT_DESC> const& layout, ID3DBlob* p_vertex_shader_byte_code) {
        for (auto const& element : layout)
            _layoutDesc += element.SemanticName;

        device.CreateInputLayout(
            layout.data(),
            layout.size(),
            p_vertex_shader_byte_code->GetBufferPointer(),
            p_vertex_shader_byte_code->GetBufferSize(),
            _inputLayout.ReleaseAndGetAddressOf()
        );
    }

    void D3D11InputLayout::Bind(ID3D11DeviceContext& context) noexcept {
        context.IASetInputLayout(_inputLayout.Get());
    }

    x_string D3D11InputLayout::GenUID(x_string const& layout_desc) {
        return x_string{typeid(D3D11InputLayout).name()} + "#" + layout_desc;
    }

    x_string D3D11InputLayout::GenUID(x_vector<D3D11_INPUT_ELEMENT_DESC> const& layout) {
        x_string layout_desc {};
        for (auto const& element : layout)
            layout_desc += element.SemanticName;

        return x_string{typeid(D3D11InputLayout).name()} + "#" + layout_desc;
    }

    x_string D3D11InputLayout::GetUID() const {
        return GenUID(_layoutDesc);
    }
}

#pragma once

namespace Engine::Graphics {
    class D3D11InputLayout {
    public:
        D3D11InputLayout(ID3D11Device& device, x_vector<D3D11_INPUT_ELEMENT_DESC> const& layout, ID3DBlob* p_vertex_shader_byte_code);

        void Bind(ID3D11DeviceContext& context) noexcept;
        [[nodiscard]] static x_string GenUID(x_string const& layout_desc);
        [[nodiscard]] static x_string GenUID(x_vector<D3D11_INPUT_ELEMENT_DESC> const& layout);
        [[nodiscard]] x_string GetUID() const;
    private:
        x_string _layoutDesc;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
    };
}


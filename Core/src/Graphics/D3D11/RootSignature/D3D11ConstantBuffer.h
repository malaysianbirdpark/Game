#pragma once

namespace Engine::Graphics {
    class D3D11ConstantBuffer {
    public:
        D3D11ConstantBuffer(
            ID3D11Device& device, 
            void const* data, 
            size_t data_size,
            bool vertex_shader_visible,
            bool pixel_shader_visible,
            bool dynamic,
            char const* tag
        );
        ~D3D11ConstantBuffer();

        void Update(ID3D11DeviceContext& context, void const* data, size_t data_size);
        void Bind(ID3D11DeviceContext& context, UINT slot);

        [[nodiscard]] static x_string GenUID(bool vsVisible, bool psVisible, bool dynamic, char const* tag);
    private:
        x_string _tag;
        bool _vsVisible {};
        bool _psVisible {};
        bool _dynamic {};
        Microsoft::WRL::ComPtr<ID3D11Buffer> _resource;
    };
}


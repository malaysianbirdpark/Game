#pragma once

namespace Engine::Graphics {
    namespace Vertex {
        class Layout;        
    }

    class D3D11PipelineStateObject {
    public:
        D3D11PipelineStateObject() = default;
        ~D3D11PipelineStateObject() = default;

        void SetInputLayout(ID3D11Device& device, x_vector<D3D11_INPUT_ELEMENT_DESC> const& layout);
        void SetPixelShader(ID3D11Device& device, char const* path);
        void SetVertexShader(ID3D11Device& device, char const* path);

        void Bind(ID3D11DeviceContext& context);
    private:
        std::shared_ptr<class D3D11InputLayout>  _inputLayout {};
        std::shared_ptr<class D3D11PixelShader>  _pixelShader {};
        std::shared_ptr<class D3D11VertexShader> _vertexShader {};
    };

}

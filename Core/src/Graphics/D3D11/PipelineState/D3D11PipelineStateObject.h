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
        void SetGeometryShader(ID3D11Device& device, char const* path);

        void Bind(ID3D11DeviceContext& context);
    private:
        Microsoft::WRL::ComPtr<ID3D11InputLayout>    _inputLayout;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>    _ps;
        Microsoft::WRL::ComPtr<ID3D11VertexShader>   _vs;
        Microsoft::WRL::ComPtr<ID3DBlob>             _vsBytesCode;
        Microsoft::WRL::ComPtr<ID3D11GeometryShader> _gs;
        //std::unique_ptr<class D3D11InputLayout>      _inputLayout {};
        //std::shared_ptr<class D3D11PixelShader>    _pixelShader {};
        //std::shared_ptr<class D3D11VertexShader>   _vertexShader {};
        //std::shared_ptr<class D3D11GeometryShader> _geometryShader {};
    };

}

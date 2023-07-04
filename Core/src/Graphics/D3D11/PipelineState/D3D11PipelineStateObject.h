#pragma once

namespace Engine::Graphics {
    namespace Vertex {
        class Layout;        
        class Buffer;
    }

    class D3D11PipelineStateObject {
    public:
        D3D11PipelineStateObject() = default;
        ~D3D11PipelineStateObject() = default;

        void SetIndexBuffer(ID3D11Device& device, x_vector<unsigned short> const& indices, char const* tag);
        void SetInputLayout(ID3D11Device& device, Vertex::Layout const& layout);
        void SetVertexBuffer(ID3D11Device& device, Vertex::Buffer const& vbuf, char const* tag);
        void SetPixelShader(ID3D11Device& device, char const* path);
        void SetVertexShader(ID3D11Device& device, char const* path);

        uint16_t GetIndexCount();

        void Bind(ID3D11DeviceContext& context);
    private:
        std::shared_ptr<class D3D11IndexBuffer>  _indexBuffer {};
        std::shared_ptr<class D3D11InputLayout>  _inputLayout {};
        std::shared_ptr<class D3D11VertexBuffer> _vertexBuffer {};
        std::shared_ptr<class D3D11PixelShader>  _pixelShader {};
        std::shared_ptr<class D3D11VertexShader> _vertexShader {};
    };

}

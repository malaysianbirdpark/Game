#pragma once

namespace Engine::Graphics {
    namespace Vertex {
        class Layout;        
        class Buffer;
    }

    class D3D11PipelineStateHolder {
        friend class D3D11PipelineStateObject;
    public:
        D3D11PipelineStateHolder() = default;
        ~D3D11PipelineStateHolder() = default;

        static std::shared_ptr<class D3D11IndexBuffer> ResolveIndexBuffer(ID3D11Device& device, x_vector<unsigned short> const& indices, char const* tag);
        static std::shared_ptr<class D3D11InputLayout> ResolveInputLayout(ID3D11Device& device, Vertex::Layout const& layout, ID3DBlob* p_vertex_shader_byte_code);  
        static std::shared_ptr<class D3D11VertexBuffer> ResolveVertexBuffer(ID3D11Device& device, Vertex::Buffer const& vbuf, char const* tag);
        static std::shared_ptr<class D3D11PixelShader> ResolvePixelShader(ID3D11Device& device, char const* path); 
        static std::shared_ptr<class D3D11VertexShader> ResolveVertexShader(ID3D11Device& device, char const* path);
    private:
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11IndexBuffer>>   _indexBuffer;
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11InputLayout>>   _inputLayout;
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11VertexBuffer>>  _vertexBuffer;
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11PixelShader>>   _pixelShader;
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11VertexShader>>  _vertexShader;
    };
}


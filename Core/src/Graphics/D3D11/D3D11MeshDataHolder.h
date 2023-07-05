#pragma once

namespace Engine::Graphics {
    namespace Vertex {
        class Buffer;
    }

    class D3D11MeshDataHolder {
    public:
        static std::shared_ptr<class D3D11IndexBuffer> ResolveIndexBuffer(ID3D11Device& device, x_vector<unsigned short> const& indices, char const* tag);
        static std::shared_ptr<class D3D11VertexBuffer> ResolveVertexBuffer(ID3D11Device& device, Vertex::Buffer const& vbuf, char const* tag);
    private:
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11IndexBuffer>>   _indexBuffer;
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11VertexBuffer>>  _vertexBuffer;
    };
}


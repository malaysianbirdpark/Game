#include "pch.h"
#include "D3D11MeshDataHolder.h"

#include "D3D11IndexBuffer.h"
#include "D3D11VertexBuffer.h"

#include "../D3DVertex.h"

std::shared_ptr<Engine::Graphics::D3D11IndexBuffer> Engine::Graphics::D3D11MeshDataHolder::ResolveIndexBuffer(ID3D11Device& device, x_vector<unsigned short> const& indices, char const* tag) {
    auto const uid {D3D11IndexBuffer::GenUID(tag)};

    if (_indexBuffer.contains(uid))
        return _indexBuffer[uid];
    return _indexBuffer[uid] = MakeShared<D3D11IndexBuffer>(device, indices, tag);
}

std::shared_ptr<Engine::Graphics::D3D11VertexBuffer> Engine::Graphics::D3D11MeshDataHolder::ResolveVertexBuffer(ID3D11Device& device, Vertex::Buffer const& vbuf, char const* tag) {
    auto const uid {D3D11VertexBuffer::GenUID(tag)};

    if (_vertexBuffer.contains(uid))
        return _vertexBuffer[uid];
    return _vertexBuffer[uid] = MakeShared<D3D11VertexBuffer>(device, vbuf, tag);
}

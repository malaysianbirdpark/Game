#include "pch.h"
#include "D3D11MeshDataHolder.h"

#include "D3D11IndexBuffer.h"
#include "D3D11VertexBuffer.h"

std::shared_ptr<Engine::Graphics::D3D11IndexBuffer> Engine::Graphics::D3D11MeshDataHolder::ResolveIndexBuffer(ID3D11Device& device, void const* data, UINT count, char const* tag) {
    auto const uid {D3D11IndexBuffer::GenUID(tag)};

    if (_indexBuffer.contains(uid))
        return _indexBuffer[uid];
    return _indexBuffer[uid] = MakeShared<D3D11IndexBuffer>(device, data, count, tag);
}

std::shared_ptr<Engine::Graphics::D3D11VertexBuffer> Engine::Graphics::D3D11MeshDataHolder::ResolveVertexBuffer(ID3D11Device& device, UINT stride, UINT byte_width, void const* data, char const* tag) {
    auto const uid {D3D11VertexBuffer::GenUID(tag)};

    if (_vertexBuffer.contains(uid))
        return _vertexBuffer[uid];
    return _vertexBuffer[uid] = MakeShared<D3D11VertexBuffer>(device, stride, byte_width, data, tag);
}

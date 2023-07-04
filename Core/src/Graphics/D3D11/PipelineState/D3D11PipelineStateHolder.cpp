#include "pch.h"
#include "D3D11PipelineStateHolder.h"

#include "D3D11PipelineStateCommon.h"

#include "..\..\D3DVertex.h"

std::shared_ptr<Engine::Graphics::D3D11IndexBuffer> Engine::Graphics::D3D11PipelineStateHolder::ResolveIndexBuffer(ID3D11Device& device, x_vector<unsigned short> const& indices, char const* tag) {
    auto const uid {D3D11IndexBuffer::GenUID(tag)};

    if (_indexBuffer.contains(uid))
        return _indexBuffer[uid];
    return _indexBuffer[uid] = MakeShared<D3D11IndexBuffer>(device, indices, tag);
}

std::shared_ptr<Engine::Graphics::D3D11InputLayout> Engine::Graphics::D3D11PipelineStateHolder::ResolveInputLayout(ID3D11Device& device, Vertex::Layout const& layout, ID3DBlob* p_vertex_shader_byte_code) {
    auto const uid {D3D11InputLayout::GenUID(layout)};

    if (_inputLayout.contains(uid))
        return _inputLayout[uid];
    return _inputLayout[uid] = MakeShared<D3D11InputLayout>(device, layout, p_vertex_shader_byte_code);
}

std::shared_ptr<Engine::Graphics::D3D11VertexBuffer> Engine::Graphics::D3D11PipelineStateHolder::ResolveVertexBuffer(ID3D11Device& device, Vertex::Buffer const& vbuf, char const* tag) {
    auto const uid {D3D11VertexBuffer::GenUID(tag)};

    if (_vertexBuffer.contains(uid))
        return _vertexBuffer[uid];
    return _vertexBuffer[uid] = MakeShared<D3D11VertexBuffer>(device, vbuf, tag);
}

std::shared_ptr<Engine::Graphics::D3D11PixelShader> Engine::Graphics::D3D11PipelineStateHolder::ResolvePixelShader(ID3D11Device& device, char const* path) {
    auto const uid {D3D11PixelShader::GenUID(path)};

    if (_pixelShader.contains(uid))
        return _pixelShader[uid];
    return _pixelShader[uid] = MakeShared<D3D11PixelShader>(device, path);
}

std::shared_ptr<Engine::Graphics::D3D11VertexShader> Engine::Graphics::D3D11PipelineStateHolder::ResolveVertexShader(ID3D11Device& device, char const* path) {
    auto const uid {D3D11IndexBuffer::GenUID(path)};

    if (_vertexShader.contains(uid))
        return _vertexShader[uid];
    return _vertexShader[uid] = MakeShared<D3D11VertexShader>(device, path);
}

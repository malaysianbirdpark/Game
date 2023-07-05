#include "pch.h"
#include "D3D11PipelineStateHolder.h"

#include "D3D11PipelineStateCommon.h"

std::shared_ptr<Engine::Graphics::D3D11InputLayout> Engine::Graphics::D3D11PipelineStateHolder::ResolveInputLayout(ID3D11Device& device, Vertex::Layout const& layout, ID3DBlob* p_vertex_shader_byte_code) {
    auto const uid {D3D11InputLayout::GenUID(layout)};

    if (_inputLayout.contains(uid))
        return _inputLayout[uid];
    return _inputLayout[uid] = MakeShared<D3D11InputLayout>(device, layout, p_vertex_shader_byte_code);
}

std::shared_ptr<Engine::Graphics::D3D11PixelShader> Engine::Graphics::D3D11PipelineStateHolder::ResolvePixelShader(ID3D11Device& device, char const* path) {
    auto const uid {D3D11PixelShader::GenUID(path)};

    if (_pixelShader.contains(uid))
        return _pixelShader[uid];
    return _pixelShader[uid] = MakeShared<D3D11PixelShader>(device, path);
}

std::shared_ptr<Engine::Graphics::D3D11VertexShader> Engine::Graphics::D3D11PipelineStateHolder::ResolveVertexShader(ID3D11Device& device, char const* path) {
    auto const uid {D3D11VertexShader::GenUID(path)};

    if (_vertexShader.contains(uid))
        return _vertexShader[uid];
    return _vertexShader[uid] = MakeShared<D3D11VertexShader>(device, path);
}

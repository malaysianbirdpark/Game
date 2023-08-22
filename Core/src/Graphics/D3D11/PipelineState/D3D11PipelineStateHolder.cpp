#include "pch.h"
#include "D3D11PipelineStateHolder.h"

#include "D3D11PipelineStateCommon.h"

std::shared_ptr<Engine::Graphics::D3D11PixelShader> Engine::Graphics::D3D11PipelineStateHolder::ResolvePixelShader(ID3D11Device& device, char const* path) {
    auto const uid {D3D11PixelShader::GenUID(path)};

    if (_pixelShader.contains(uid))
        return _pixelShader[uid];
    return _pixelShader[uid] = std::move(MakeShared<D3D11PixelShader>(device, path));
}

std::shared_ptr<Engine::Graphics::D3D11VertexShader> Engine::Graphics::D3D11PipelineStateHolder::ResolveVertexShader(ID3D11Device& device, char const* path) {
    auto const uid {D3D11VertexShader::GenUID(path)};

    if (_vertexShader.contains(uid))
        return _vertexShader[uid];
    return _vertexShader[uid] = std::move(MakeShared<D3D11VertexShader>(device, path));
}

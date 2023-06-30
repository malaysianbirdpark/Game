#include "pch.h"
#include "D3D11PipelineStateObject.h"

#include "D3D11PipelineStateHolder.h"
#include "D3D11PipelineStateCommon.h"

#include "../../Vertex.h"

void Engine::Graphics::D3D11PipelineStateObject::SetIndexBuffer(ID3D11Device& device, x_vector<unsigned short> const& indices, char const* tag) {
    _indexBuffer = D3D11PipelineStateHolder::ResolveIndexBuffer(device, indices, tag);
}

void Engine::Graphics::D3D11PipelineStateObject::SetInputLayout(ID3D11Device& device, Vertex::Layout const& layout) {
    if (_vertexShader == nullptr) {
        GRAPHICS_ERROR("Set SetVertexShader First!!!!!!!");
        CRASH();
    }

    _inputLayout = D3D11PipelineStateHolder::ResolveInputLayout(device, layout, _vertexShader->GetByteCode());
}

void Engine::Graphics::D3D11PipelineStateObject::SetVertexBuffer(ID3D11Device& device, Vertex::Buffer const& vbuf, char const* tag) {
    _vertexBuffer = D3D11PipelineStateHolder::ResolveVertexBuffer(device, vbuf, tag);
}

void Engine::Graphics::D3D11PipelineStateObject::SetPixelShader(ID3D11Device& device, char const* path) {
    _pixelShader = D3D11PipelineStateHolder::ResolvePixelShader(device, path);
}

void Engine::Graphics::D3D11PipelineStateObject::SetVertexShader(ID3D11Device& device, char const* path) {
    _vertexShader = D3D11PipelineStateHolder::ResolveVertexShader(device, path); 
}

void Engine::Graphics::D3D11PipelineStateObject::Bind(ID3D11DeviceContext& context) {
    context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    _indexBuffer->Bind(context);
    _inputLayout->Bind(context);
    _vertexBuffer->Bind(context);
    _pixelShader->Bind(context);
    _vertexShader->Bind(context);
}


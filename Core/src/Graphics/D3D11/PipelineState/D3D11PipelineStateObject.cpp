#include "pch.h"
#include "D3D11PipelineStateObject.h"

#include "D3D11PipelineStateHolder.h"
#include "D3D11PipelineStateCommon.h"

void Engine::Graphics::D3D11PipelineStateObject::SetInputLayout(ID3D11Device& device, x_vector<D3D11_INPUT_ELEMENT_DESC> const& layout) {
    if (_vertexShader == nullptr) {
        GRAPHICS_ERROR("Set SetVertexShader First!!!!!!!");
        CRASH();
    }

    _inputLayout = D3D11PipelineStateHolder::ResolveInputLayout(device, layout, _vertexShader->GetByteCode());
}

void Engine::Graphics::D3D11PipelineStateObject::SetPixelShader(ID3D11Device& device, char const* path) {
    _pixelShader = D3D11PipelineStateHolder::ResolvePixelShader(device, path);
}

void Engine::Graphics::D3D11PipelineStateObject::SetVertexShader(ID3D11Device& device, char const* path) {
    _vertexShader = D3D11PipelineStateHolder::ResolveVertexShader(device, path); 
}

void Engine::Graphics::D3D11PipelineStateObject::Bind(ID3D11DeviceContext& context) {
    _inputLayout->Bind(context);
    _vertexShader->Bind(context);
    _pixelShader->Bind(context);
}


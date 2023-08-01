#include "pch.h"
#include "D3D11ConstantBuffer.h"

#include "D3D11TransformMVP.h"
#include "D3D11LightDirectional.h"

void Engine::Graphics::BindD3D11TransformMVP(ID3D11DeviceContext& context, D3D11TransformMVP const& target) {
    target.Bind(context);
}

void Engine::Graphics::BindD3D11LightDirectional(ID3D11DeviceContext& context, D3D11LightDirectional const& target) {
    target.Bind(context);
}





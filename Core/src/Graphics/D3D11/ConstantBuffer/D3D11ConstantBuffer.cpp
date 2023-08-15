#include "pch.h"
#include "D3D11ConstantBuffer.h"

#include "D3D11CamPosition.h"
#include "D3D11LightDirectional.h"

void Engine::Graphics::BindD3D11CamPosition(ID3D11DeviceContext& context, D3D11CamPosition& target) {
    target.Bind(context); 
}


void Engine::Graphics::BindD3D11LightDirectional(ID3D11DeviceContext& context, D3D11LightDirectional& target) {
    target.Bind(context);
}

void Engine::Graphics::UpdateD3D11CamPosition(D3D11CamPosition& target) {
    target.Update();    
}

void Engine::Graphics::UpdateD3D11LightDirectional(D3D11LightDirectional& target) {
    target.Update();
}


#include "pch.h"
#include "D3D11RenderStrategy.h"

#include "D3D11SolidStrategy.h"
#include "D3D11TexturedStrategy.h"

void Engine::Graphics::InitRenderStrategies() {
    D3D11SolidStrategy::Init();
    D3D11TexturedStrategy::Init();
}

void Engine::Graphics::RenderD3D11SolidStrategy(ID3D11DeviceContext& context, D3D11SolidStrategy const& strategy, D3D11Mesh* mesh) {
    strategy.Render(context, *mesh);
}

void Engine::Graphics::RenderD3D11TexturedStrategy(ID3D11DeviceContext& context, D3D11TexturedStrategy const& strategy, D3D11Mesh* mesh) {
    strategy.Render(context, *mesh); 
}







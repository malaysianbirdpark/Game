#include "pch.h"
#include "D3D11PhongStrategy.h"

#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

void Engine::Graphics::D3D11PhongStrategy::Init() {
    GRAPHICS_INFO("Phong-Tex Strategy Init()");
    _pso = D3D11PSOLibrary::ResolvePSO("phong");
}

void Engine::Graphics::D3D11PhongStrategy::Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh, D3D11Material& material) {
    mesh.Bind(context);
    material.BindPhong(context);
    _pso->Bind(context);

    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}

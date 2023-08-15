#include "pch.h"
#include "D3D11PhongTexStrategy.h"

#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

#include "Graphics/D3D11/ShaderResource/D3D11DiffuseMap.h"

Engine::Graphics::D3D11PhongTexStrategy Engine::Graphics::D3D11PhongTexStrategy::GetInstance() {
    return D3D11PhongTexStrategy{};
}

void Engine::Graphics::D3D11PhongTexStrategy::Init() {
    GRAPHICS_INFO("Phong-Tex Strategy Init()");
    _pso = D3D11PSOLibrary::ResolvePSO("phong_tex");
}

void Engine::Graphics::D3D11PhongTexStrategy::Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh, D3D11Material const& material) const {
    _pso->Bind(context);

    for (auto const& sr : material.GetShaderResources()) {
        if (D3D11DiffuseMap const* diffuse_texture {std::get_if<D3D11DiffuseMap>(sr.get())}) {
            diffuse_texture->Bind(context);  
        }
    }

    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);

}

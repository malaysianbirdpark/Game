#include "pch.h"
#include "D3D11PhongTexNormStrategy.h"

#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

#include "Graphics/D3D11/ShaderResource/D3D11DiffuseMap.h"
#include "Graphics/D3D11/ShaderResource/D3D11NormalMap.h"

void Engine::Graphics::D3D11PhongTexNormStrategy::Init() {
    GRAPHICS_INFO("Phong-Tex-Norm Strategy Init()");
    _pso = D3D11PSOLibrary::ResolvePSO("phong_tex_norm");
}

void Engine::Graphics::D3D11PhongTexNormStrategy::Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh, D3D11Material const& material) {
    mesh.Bind(context);
    _pso->Bind(context);

    for (auto const& sr : material.GetShaderResources()) {
        if (auto const* diffuse_map {std::get_if<std::shared_ptr<D3D11DiffuseMap>>(&sr)}) {
            diffuse_map->get()->Bind(context);  
        }
        else if (auto const* normal_map {std::get_if<std::shared_ptr<D3D11NormalMap>>(&sr)}) {
            normal_map->get()->Bind(context);  
        }
    }

    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}


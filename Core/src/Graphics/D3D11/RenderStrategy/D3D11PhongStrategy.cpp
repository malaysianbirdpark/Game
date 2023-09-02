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

    for (auto const& sr : material.GetShaderResources()) {
        if (auto const* diffuse_map {std::get_if<std::shared_ptr<D3D11DiffuseMap>>(&sr)}) {
            diffuse_map->get()->Bind(context);  
        }
        else if (auto const* specular_map {std::get_if<std::shared_ptr<D3D11SpecularMap>>(&sr)}) {
            specular_map->get()->Bind(context);  
        }
        else if (auto const* normal_map {std::get_if<std::shared_ptr<D3D11NormalMap>>(&sr)}) {
            normal_map->get()->Bind(context);  
        }
        else if (auto const* height_map {std::get_if<std::shared_ptr<D3D11HeightMap>>(&sr)}) {
            height_map->get()->Bind(context);  
        }
        else if (auto const* metallic_map {std::get_if<std::shared_ptr<D3D11MetallicMap>>(&sr)}) {
            metallic_map->get()->Bind(context);  
        }
        else if (auto const* roughness_map {std::get_if<std::shared_ptr<D3D11RoughnessMap>>(&sr)}) {
            roughness_map->get()->Bind(context);  
        }
    }

    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}

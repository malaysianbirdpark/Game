#include "pch.h"
#include "D3D11Cubemap.h"

#include "Graphics/D3DCamera.h"
#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

Engine::Graphics::D3D11Cubemap::D3D11Cubemap(ID3D11Device& device, DirectX::XMMATRIX proj)
    : _vertex{device, sizeof(CubeMapVertex), sizeof(CubeMapVertex) * sizeof(cube_vertices), cube_vertices.data(), "cubemap"},
      _index{device, cube_indices.data(), cube_indices.size(), "cubemap"},
      _transform{device, DirectX::XMMatrixIdentity(), D3DCamera::GetView(), proj}
{
    _pso = D3D11PSOLibrary::ResolvePSO("cubemap");

    AddSDRTexture(device, "Assets/CubeMapTextures/SantaMaria.dds");
    AddSDRTexture(device, "Assets/CubeMapTextures/FortPoint.dds"); 
    AddSDRTexture(device, "Assets/CubeMapTextures/Atrium_specularIBL.dds", "Assets/CubeMapTextures/Atrium_diffuseIBL.dds");
    AddSDRTexture(device, "Assets/CubeMapTextures/Garage_specularIBL.dds", "Assets/CubeMapTextures/Garage_diffuseIBL.dds");
    AddHDRTexture(
        device, 
        "Assets/CubeMapTextures/DayEnvironmentEnvHDR.dds",
        "Assets/CubeMapTextures/DayEnvironmentSpecularHDR.dds",
        "Assets/CubeMapTextures/DayEnvironmentDiffuseHDR.dds"
    );
}

void Engine::Graphics::D3D11Cubemap::AddSDRTexture(ID3D11Device& device, char const* specular_path) {
    _textures.push_back(D3D11SDRCubemapTexture{device, specular_path});
}

void Engine::Graphics::D3D11Cubemap::AddSDRTexture(ID3D11Device& device, char const* specular_path, char const* diffuse_path) {
    _textures.push_back(D3D11SDRCubemapTexture{device, specular_path, specular_path, diffuse_path});
}

void Engine::Graphics::D3D11Cubemap::AddHDRTexture(ID3D11Device& device, char const* env_path, char const* specular_path, char const* diffuse_path) {
    _textures.push_back(D3D11HDRCubemapTexture{device, env_path, specular_path, diffuse_path});
}

void Engine::Graphics::D3D11Cubemap::SetTexture(uint8_t idx) {
    CORE_ASSERT(idx < _textures.size(), "Provided Cubemap texture index is invalid");
    _selectedTexture = idx;
}

int& Engine::Graphics::D3D11Cubemap::TextureIndex() {
    return _selectedTexture;
}

void Engine::Graphics::D3D11Cubemap::Update(float const dt, DirectX::XMMATRIX view, DirectX::XMMATRIX proj) {
    _transform.SetView(view);
    _transform.SetProj(proj);
}

void Engine::Graphics::D3D11Cubemap::Bind(ID3D11DeviceContext& context) {
    std::visit(BindCubemapTexture{context}, _textures[_selectedTexture]);
}

void Engine::Graphics::D3D11Cubemap::Render(ID3D11DeviceContext& context) {
    context.IASetPrimitiveTopology(_topology);
    _vertex.Bind(context);
    _index.Bind(context);

    _pso->Bind(context);
    Bind(context);

    _transform.Update(context);
    _transform.Bind(context);

    context.DrawIndexed(cube_indices.size(), 0u, 0u);
}

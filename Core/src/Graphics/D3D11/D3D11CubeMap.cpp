#include "pch.h"
#include "D3D11CubeMap.h"

#include "Graphics/D3DCamera.h"
#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

void Engine::Graphics::D3D11CubeMap::Init(ID3D11Device& device, DirectX::XMMATRIX proj) {
    _vertex = MakeUnique<D3D11VertexBuffer>(device, sizeof(CubeMapVertex), sizeof(CubeMapVertex) * cube_vertices.size(), cube_vertices.data(), "cubemap");
    _index = MakeUnique<D3D11IndexBuffer>(device, cube_indices.data(), cube_indices.size(), "cubemap");

    _pso = D3D11PSOLibrary::ResolvePSO("cubemap");
    _transform = MakeUnique<D3D11TransformMVP>(device, DirectX::XMMatrixIdentity(), D3DCamera::GetView(), proj);

    D3D11_SAMPLER_DESC sd {};
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sd.MinLOD = 0;
    sd.MaxLOD = D3D11_FLOAT32_MAX;

    device.CreateSamplerState(&sd, _sampler.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11CubeMap::AddTexture(ID3D11Device& device, char const* path) {
    _textures.push_back(
        {
            D3D11ShaderResourceHolder::Resolve(device, ShaderResourceTypes::D3D11SpecularCubemapTexture, path),
            D3D11ShaderResourceHolder::Resolve(device, ShaderResourceTypes::D3D11SpecularCubemapTexture, path)
        }
    );
}

void Engine::Graphics::D3D11CubeMap::AddTexture(ID3D11Device& device, char const* diffuse_path, char const* specular_path) {
    _textures.push_back(
        {
            D3D11ShaderResourceHolder::Resolve(device, ShaderResourceTypes::D3D11DiffuseCubemapTexture, diffuse_path),
            D3D11ShaderResourceHolder::Resolve(device, ShaderResourceTypes::D3D11SpecularCubemapTexture, specular_path)
        }
    );
}

void Engine::Graphics::D3D11CubeMap::SetTexture(uint8_t idx) {
    CORE_ASSERT(idx < _textures.size(), "Provided Cubemap texture index is invalid");
    _selectedTexture = idx;
}

int& Engine::Graphics::D3D11CubeMap::TextureIndex() {
    return _selectedTexture;
}

void Engine::Graphics::D3D11CubeMap::Update(float const dt, DirectX::XMMATRIX view, DirectX::XMMATRIX proj) {
    _transform->SetView(view);
    _transform->SetProj(proj);
}

void Engine::Graphics::D3D11CubeMap::BindDiffuse(ID3D11DeviceContext& context) {
    std::visit(BindShaderResource{context}, _textures[_selectedTexture].first);
}

void Engine::Graphics::D3D11CubeMap::BindSpecular(ID3D11DeviceContext& context) {
    std::visit(BindShaderResource{context}, _textures[_selectedTexture].second);
}

void Engine::Graphics::D3D11CubeMap::Render(ID3D11DeviceContext& context) {
    context.IASetPrimitiveTopology(_topology);
    _vertex->Bind(context);
    _index->Bind(context);

    _pso->Bind(context);
    std::visit(BindShaderResource{context}, _textures[_selectedTexture].second);
    context.PSSetSamplers(1u, 1u, _sampler.GetAddressOf());

    _transform->Update(context);
    _transform->Bind(context);

    context.DrawIndexed(cube_indices.size(), 0u, 0u);
}

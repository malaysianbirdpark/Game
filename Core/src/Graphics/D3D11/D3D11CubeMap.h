#pragma once

#include "ConstantBuffer/D3D11TransformMVP.h"
#include "Graphics/D3D11/SceneGraph/D3D11VertexBuffer.h"
#include "Graphics/D3D11/SceneGraph/D3D11IndexBuffer.h"
#include "ShaderResource/D3D11DiffuseCubemapTexture.h"
#include "ShaderResource/D3D11ShaderResource.h"

namespace Engine::Graphics {
    class D3D11CubeMap {
        enum {
            edge = 60,
        };
        struct CubeMapVertex {
            DirectX::XMFLOAT3 pos;
        };
    public:
        static void Init(ID3D11Device& device, DirectX::XMMATRIX proj);
        static void AddTexture(ID3D11Device& device, char const* path);
        static void AddTexture(ID3D11Device& device, char const* diffuse_path, char const* specular_path);
        static void SetTexture(uint8_t idx);
        static int& TextureIndex();

        static void Update(float const dt, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
        static void BindDiffuse(ID3D11DeviceContext& context);
        static void BindSpecular(ID3D11DeviceContext& context);
        static void Render(ID3D11DeviceContext& context);
    private:
        inline static x_array<CubeMapVertex, 24> cube_vertices {{
            {.pos = {-edge, edge, -edge}},
            {.pos = {-edge, edge, edge}},
            {.pos = {edge, edge, edge}},
            {.pos = {edge, edge, -edge}},

            {.pos = {-edge, -edge, -edge}},
            {.pos = {edge, -edge, -edge}},
            {.pos = {edge, -edge, edge}},
            {.pos = {-edge, -edge, edge}},

            {.pos = {-edge, -edge, -edge}},
            {.pos = {-edge, edge, -edge}},
            {.pos = {edge, edge, -edge}},
            {.pos = {edge, -edge, -edge}},

            {.pos = {-edge, -edge, edge}},
            {.pos = {edge, -edge, edge}},
            {.pos = {edge, edge, edge}},
            {.pos = {-edge, edge, edge}},

            {.pos = {-edge, -edge, edge}},
            {.pos = {-edge, edge, edge}},
            {.pos = {-edge, edge, -edge}},
            {.pos = {-edge, -edge, -edge}},

            {.pos = {edge, -edge, edge}},
            {.pos = {edge, -edge, -edge}},
            {.pos = {edge, edge, -edge}},
            {.pos = {edge, edge, edge}},
        }};
        inline static x_array<uint16_t, 36> cube_indices {
            23, 22, 20, 22, 21, 20,
            19, 18, 16, 18, 17, 16,
            15, 14, 12, 14, 13, 12,
            11, 10,  8, 10,  9,  8,
             7,  6,  4,  6,  5,  4,
             3,  2,  0,  2,  1,  0
        };
    private:
        inline static D3D11_PRIMITIVE_TOPOLOGY                          _topology {D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST};
        inline static std::unique_ptr<class D3D11VertexBuffer>          _vertex;
        inline static std::unique_ptr<class D3D11IndexBuffer>           _index;

        inline static std::shared_ptr<class D3D11PipelineStateObject>   _pso;
        inline static int                                               _selectedTexture {};
        inline static Microsoft::WRL::ComPtr<ID3D11SamplerState>        _sampler {};

        inline static std::unique_ptr<D3D11TransformMVP>                _transform;

        inline static x_vector<
            std::pair<
                D3D11ShaderResource,
                D3D11ShaderResource
            >
        > _textures;
    };
}


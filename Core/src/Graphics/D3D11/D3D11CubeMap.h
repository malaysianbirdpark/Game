#pragma once

#include "ConstantBuffer/D3D11TransformCB.h"
#include "Graphics/D3D11/SceneGraph/D3D11VertexBuffer.h"
#include "Graphics/D3D11/SceneGraph/D3D11IndexBuffer.h"
#include "Graphics/D3D11/ShaderResource/D3D11CubemapTexture.h"

namespace Engine::Graphics {
    class D3D11Cubemap {
        enum {
            side = 3000,
        };
        struct CubeMapVertex {
            DirectX::XMFLOAT3 pos;
        };
    public:
        D3D11Cubemap(ID3D11Device& device, ID3D11DeviceContext& context, DirectX::XMMATRIX proj);

        static void AddSDRTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* specular_path);
        static void AddSDRTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* specular_path, char const* diffuse_path);
        static void AddHDRTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path, char const* diffuse_path);
        static void AddHDRTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path, char const* diffuse_path, char const* brdf_path);
        static void SetTexture(uint8_t idx);
        static int& TextureIndex();

        void Update(float const dt);
        static void Bind(ID3D11DeviceContext& context);
        void Render(ID3D11DeviceContext& context);
    private:
        inline static x_array<CubeMapVertex, 24> cube_vertices {{
            {.pos = {-side, side, -side}},
            {.pos = {-side, side, side}},
            {.pos = {side, side, side}},
            {.pos = {side, side, -side}},

            {.pos = {-side, -side, -side}},
            {.pos = {side, -side, -side}},
            {.pos = {side, -side, side}},
            {.pos = {-side, -side, side}},

            {.pos = {-side, -side, -side}},
            {.pos = {-side, side, -side}},
            {.pos = {side, side, -side}},
            {.pos = {side, -side, -side}},

            {.pos = {-side, -side, side}},
            {.pos = {side, -side, side}},
            {.pos = {side, side, side}},
            {.pos = {-side, side, side}},

            {.pos = {-side, -side, side}},
            {.pos = {-side, side, side}},
            {.pos = {-side, side, -side}},
            {.pos = {-side, -side, -side}},

            {.pos = {side, -side, side}},
            {.pos = {side, -side, -side}},
            {.pos = {side, side, -side}},
            {.pos = {side, side, side}},
        }};
        inline static x_array<uint32_t, 36> cube_indices {
            23, 22, 20, 22, 21, 20,
            19, 18, 16, 18, 17, 16,
            15, 14, 12, 14, 13, 12,
            11, 10,  8, 10,  9,  8,
             7,  6,  4,  6,  5,  4,
             3,  2,  0,  2,  1,  0
        };
    private:
        D3D11_PRIMITIVE_TOPOLOGY                          _topology {D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST};
        D3D11VertexBuffer                                 _vertex;
        D3D11IndexBuffer                                  _index;

        std::shared_ptr<class D3D11PipelineStateObject>   _pso;
        inline static int                                 _selectedTexture {};

        D3D11TransformCB                                  _transform;

        inline static x_vector<D3D11CubemapTexture>       _textures;
    };
}


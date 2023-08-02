#pragma once

#include <assimp/scene.h>

namespace Engine::Graphics {
    class D3D11VertexBuffer;
    class D3D11IndexBuffer;

    class D3D11ModelQuery {
        enum {
            Position3D      = 0b1,
            Normal          = 0b10,
            Tangent         = 0b100,
            Texture2D       = 0b1000,
            Texture3D       = 0b10000,
            DiffuseColor    = 0b100000,
            SpecularColor   = 0b1000000,
            SkinningIndex   = 0b10000000,
            SkinningWeight  = 0b100000000,
            END             = SkinningWeight << 1,
        };
    public:
        D3D11ModelQuery() = default;

        static void Init(ID3D11Device& device, DirectX::XMMATRIX const& proj);

        using vb = std::shared_ptr<D3D11VertexBuffer>;
        using ib = std::shared_ptr<D3D11IndexBuffer>;
        std::tuple<x_string, vb, ib> Cook(ID3D11Device& device, aiMesh const* ai_mesh, aiMaterial const* ai_material);
    private:
        void QueryVertexLayout(aiMesh const* ai_mesh);
        void QueryMaterial(aiMaterial const* ai_material, x_string& tag);

        void CookVertexLayout(x_string& tag);
        x_vector<int8_t> CookVertex(aiMesh const* ai_mesh, size_t idx);
        std::pair<vb, ib> CookVertexBuffer(ID3D11Device& device, aiMesh const* ai_mesh);
    private:
        x_vector<int8_t>                    _vertices {};
        x_vector<D3D11_INPUT_ELEMENT_DESC>  _layout {};
    private:
        uint16_t                            _vertexFlag;
        uint16_t                            _materialFlag;
        UINT                                _stride {0u};
    private:
        inline static bool _initiated {false};
    };
}


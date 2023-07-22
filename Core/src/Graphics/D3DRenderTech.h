#pragma once

#include <assimp/scene.h>

namespace Engine::Graphics {
    class D3D11VertexBuffer;
    class D3D11IndexBuffer;

    class D3DRenderTech {
        enum {
            Position3D = 0b1,
            Normal     = 0b10,
            Tangent    = 0b100,
            BiTangent  = 0b1000,
            Texture2D  = 0b10000,
            END        = Texture2D << 1,
        };
    public:
        D3DRenderTech() = default;

        static void Init(ID3D11Device& device, DirectX::XMMATRIX const& proj);

        void QueryVertexLayout(aiMesh const* ai_mesh);
        void QueryMaterialProp(aiMesh const* ai_mesh, aiMaterial const* ai_material);

        using vb = std::shared_ptr<D3D11VertexBuffer>;
        using ib = std::shared_ptr<D3D11IndexBuffer>;
        std::pair<vb, ib> Cook(ID3D11Device& device, aiMesh const* ai_mesh, aiMaterial const* ai_material);
    private:
        void CookVertexLayout();
        x_vector<int8_t> CookVertex(aiMesh const* ai_mesh, size_t idx);
        std::pair<vb, ib> CookVertexBuffer(ID3D11Device& device, aiMesh const* ai_mesh);
    private:
        x_vector<int8_t>                    _vertices {};
        x_vector<D3D11_INPUT_ELEMENT_DESC>  _layout {};
    private:
        uint16_t _vertexFlag;
        uint16_t _materialFlag;
        UINT     _stride {0u};
    private:
        inline static bool _initiated {false};
    };
}


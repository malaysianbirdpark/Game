#pragma once

#include <assimp/scene.h>

namespace Engine::Graphics {
    class D3D11VertexAttribute {
    public:
        static x_vector<uint8_t> GetBuffer(aiMesh const* ai_mesh, x_string const& vertex_format);
        static UINT GetStride(x_string const& vertex_format);
        static UINT GeyByteWidth(x_string const& vertex_format, size_t number_of_vertices);
    private:
        static void ParseVertex_1P(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer);
        static void ParseVertex_1P1N(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer);
        static void ParseVertex_1P1N1UV(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer);
        static void ParseVertex_1P1N1T(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer);
        static void ParseVertex_1P1N1T1UV(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer);
    private:
        struct _1P {
            DirectX::XMFLOAT3 _pos3d;
        };

        struct _1P1N {
            DirectX::XMFLOAT3 _pos3d;
            DirectX::XMFLOAT3 _norm3d;
        };

        struct _1P1N1UV {
            DirectX::XMFLOAT3 _pos3d;
            DirectX::XMFLOAT3 _norm3d;
            DirectX::XMFLOAT2 _uv2d;
        };

        struct _1P1N1T {
            DirectX::XMFLOAT3 _pos3d;
            DirectX::XMFLOAT3 _norm3d;
            DirectX::XMFLOAT3 _tan3d;
            DirectX::XMFLOAT3 _bitan3d;
        };

        struct _1P1N1T1UV {
            DirectX::XMFLOAT3 _pos3d;
            DirectX::XMFLOAT3 _norm3d;
            DirectX::XMFLOAT3 _tan3d;
            DirectX::XMFLOAT3 _bitan3d;
            DirectX::XMFLOAT2 _uv2d;
        };
    private:
        inline static x_unordered_map<x_string, size_t> _strideTable {
            {"1p", sizeof(_1P)},
            {"1p1n", sizeof(_1P1N)},
            {"1p1n1uv", sizeof(_1P1N1UV)},
            {"1p1n1t", sizeof(_1P1N1T)},
            {"1p1n1t1uv", sizeof(_1P1N1T1UV)},
        };

        inline static x_unordered_map<x_string, std::function<void(aiMesh const*&, x_vector<uint8_t>&)>> _parseFunctionTable {
            {"1p", &D3D11VertexAttribute::ParseVertex_1P},
            {"1p1n", &D3D11VertexAttribute::ParseVertex_1P1N},
            {"1p1n1uv", &D3D11VertexAttribute::ParseVertex_1P1N1UV},
            {"1p1n1t", &D3D11VertexAttribute::ParseVertex_1P1N1T},
            {"1p1n1t1uv", &D3D11VertexAttribute::ParseVertex_1P1N1T1UV},
        };
    public:
        inline static x_vector<D3D11_INPUT_ELEMENT_DESC> _1pLayout {
            {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        };

        inline static x_vector<D3D11_INPUT_ELEMENT_DESC> _1p1nLayout {
            {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0u}
        };

        inline static x_vector<D3D11_INPUT_ELEMENT_DESC> _1p1n1uvLayout {
            {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        };

        inline static x_vector<D3D11_INPUT_ELEMENT_DESC> _1p1n1tLayout {
            {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"TANGENT", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"BINORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0u},
        };

        inline static x_vector<D3D11_INPUT_ELEMENT_DESC> _1p1n1t1uvLayout {
            {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"TANGENT", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"BINORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0u},
            {"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 4, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        };
    };
}


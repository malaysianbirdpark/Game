#pragma once

#include <bitset>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "D3D11SceneGraph.h"
#include "Graphics/D3D11/ShaderResource/D3D11ShaderResource.h"

#include "Graphics/D3D11/ConstantBuffer/MaterialConstants/D3D11MaterialConstants.h"
#include "Graphics/D3D11/ConstantBuffer/D3D11VertexShaderConstants.h"

namespace Engine::Graphics {
    class D3D11InputLayout;
    class D3D11VertexBuffer;
    class D3D11IndexBuffer;
    class D3D11PipelineStateObject;

    struct SceneTransformParameters {
        float x {};
        float y {};
        float z {};
        float roll {};
        float pitch {};
        float yaw {};
        float scale {1.0f};
    };

    class D3D11Material {
        friend class D3D11SceneGraph;
    public:
        D3D11Material(ID3D11Device& device);
        D3D11Material(D3D11Material const& other);
        [[nodiscard]] x_vector<D3D11ShaderResource> const&  GetShaderResources() const;
        [[nodiscard]] D3D11PhongConstants* const            GetPhongConstants();
        [[nodiscard]] D3D11UnrealPBRConstants* const        GetUnrealPBRConstants();
        [[nodiscard]] D3D11SolidConstants* const            GetSolidConstants();
        [[nodiscard]] D3D11VertexShaderConstants* const     GetVSConstants();

        void                                                BindVertexShaderConstants(ID3D11DeviceContext& context);
        void                                                BindSolid(ID3D11DeviceContext& context);
        void                                                BindPhong(ID3D11DeviceContext& context);
        void                                                BindUnrealPBR(ID3D11DeviceContext& context);

        void                                                CopyPhongConstants(D3D11Material const& source);
        void                                                CopyUnrealPBRConstants(D3D11Material const& source);
        void                                                CopySolidConstants(D3D11Material const& source);
        void                                                CopyVSConstants(D3D11Material const& source);

        x_string                                            GetTextureInfo() const;

        void                                                AddOrRelplaceTexture(ID3D11Device& device, ID3D11DeviceContext& context, ShaderResourceTypes type, char const* path);
    private:
        DirectX::XMFLOAT4  _emissiveColor;
        DirectX::XMFLOAT4  _albedoColor;
        DirectX::XMFLOAT4  _roughness;
        float              _transparencyFactor;
        float              _alphaTest;
        float              _metallicFactor;

        D3D11VertexShaderConstants      _vertexShaderConstants;
        D3D11PhongConstants             _phongConstants;
        D3D11UnrealPBRConstants         _unrealPBRConstants;
        D3D11SolidConstants             _solidConstants;
        x_vector<D3D11ShaderResource>   _srs;
        std::bitset<16>                 _textureTypes;
    };

    class D3D11Mesh {
        friend class D3D11SceneGraph;
        friend class D3D11SceneHolder;
    public:
        D3D11Mesh() = default;
        D3D11Mesh(
            std::shared_ptr<D3D11VertexBuffer>& vertex_buffer,
            std::shared_ptr<D3D11IndexBuffer>& index_buffer,
            D3D11_PRIMITIVE_TOPOLOGY topology
        );
        D3D11Mesh(D3D11Mesh const& other);

        void Bind(ID3D11DeviceContext& context) const;

        [[nodiscard]] UINT GetIndexCount() const;
    private:
        std::shared_ptr<D3D11VertexBuffer>            _vertexBuffer;
        std::shared_ptr<D3D11IndexBuffer>             _indexBuffer; 
        D3D11_PRIMITIVE_TOPOLOGY                      _topology;
    };

    struct D3D11SceneNode {
         int32_t _parent;
         int32_t _firstChild;
         int32_t _firstSibling;
         int32_t _lastSibling;
         int32_t _level;

         int32_t _nextSibling {-1};
    };

    class D3D11SceneGraph {
        friend class D3D11DefaultObject;
        friend class D3D11ConcreteLight;
        friend class D3D11MirrorObject;
        friend class D3D11Mesh;
        friend class D3D11ImGuiRenderer;

        static constexpr int MAX_NODE_LEVEL {20};
    public:
        D3D11SceneGraph(ID3D11Device& device, ID3D11DeviceContext& context, char const* path);

        // Copy ctor
        D3D11SceneGraph(D3D11SceneGraph const& other);

        D3D11SceneGraph Clone();

        void       MarkAsTransformed(int32_t node);
        void       MarkAsPhongMaterialEdited(int32_t node);
        void       MarkAsUnrealPBRMaterialEdited(int32_t node);
        void       MarkAsSolidMaterialEdited(int32_t node);
        void       MarkAsVSConstantEdited(int32_t node);

        void       SetRenderStrategies(int32_t node, int strategy);

        D3D11SceneNode&                     GetNodeAt(int32_t node);
        SceneTransformParameters&           GetTransformParamAt(int32_t node);
        char const*                         GetNameAt(int32_t node);
        D3D11Material&                      GetMaterialAt(int32_t node);
        int32_t&                            GetRenderStrategyAt(int32_t node);
        int32_t                             GetClosestMaterialConstant(int32_t node);

        void                                Update();
        void                                AddMaterial(ID3D11Device& device);
    private:
        void                                RecalculateGlobalTransforms();
        void                                UpdatePhongMaterialConstants();
        void                                UpdateUnrealPBRMaterialConstants();
        void                                UpdateSolidMaterialConstants();
        void                                UpdateVertexShaderConstants();
    private:
        int32_t                              ParseNode(int32_t parent_id, int32_t level, aiScene const* ai_scene, aiNode const* ai_node);
        [[nodiscard]] static D3D11Mesh       ParseMesh(ID3D11Device& device, aiMesh const* ai_mesh);
        [[nodiscard]] static D3D11Material   ParseMaterial(ID3D11Device& device, ID3D11DeviceContext& context, aiMaterial const* ai_material, char const* base_path);
        using VertexData = std::pair<std::shared_ptr<D3D11VertexBuffer>, std::shared_ptr<D3D11IndexBuffer>>;
        [[nodiscard]] static VertexData      ParseVertexData(ID3D11Device& device, aiMesh const* ai_mesh, x_string const& vertex_format);
    private:
        int32_t                              AddNode(int32_t parent_id, int32_t level, DirectX::XMMATRIX const& local_transform = DirectX::XMMatrixIdentity());
    private:
        x_vector<D3D11Mesh>                  _mesh {};
        x_vector<D3D11Material>              _material {};
        x_vector<D3D11SceneNode>             _tree;
        x_vector<x_string>                   _nodeNames {};
    private:
        x_vector<int32_t>                    _renderStrategy;
        x_vector<SceneTransformParameters>   _transforms;
        x_vector<DirectX::XMFLOAT4X4>        _globalTransforms;
        x_vector<DirectX::XMFLOAT4X4>        _localTransforms;
    public:
        x_unordered_map<uint32_t, uint32_t>  _nodeId_to_meshId {};
        x_unordered_map<uint32_t, uint32_t>  _nodeId_to_materialId {};
        x_unordered_map<uint32_t, uint32_t>  _nodeId_to_namesId {};
    private:
        int32_t                              _recentlyUpdatedPhongMaterial {-1};
        int32_t                              _recentlyUpdatedUnrealPBRMaterial {-1};
        int32_t                              _recentlyUpdatedSolidMaterial {-1};
        int32_t                              _recentlyUpdatedVSConstants   {-1};
    private:
        // For the Update
        x_vector<int32_t>                    _transformed[MAX_NODE_LEVEL] {};
        x_vector<int32_t>                    _phongMaterialEdited[MAX_NODE_LEVEL] {};
        x_vector<int32_t>                    _unrealPBRMaterialEdited[MAX_NODE_LEVEL] {};
        x_vector<int32_t>                    _solidMaterialEdited[MAX_NODE_LEVEL] {};
        x_vector<int32_t>                    _VSCEdited[MAX_NODE_LEVEL] {};
    };

    std::string   process_ai_path(char const* base_path, char const* ai_path);
}


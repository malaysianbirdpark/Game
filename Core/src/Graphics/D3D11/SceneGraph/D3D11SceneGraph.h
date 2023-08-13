#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/D3D11/ShaderResource/D3D11ShaderResource.h"
#include "Graphics/D3D11/RenderStrategy/D3D11RenderStrategy.h"

namespace Engine::Graphics {
    class D3D11InputLayout;
    class D3D11VertexBuffer;
    class D3D11IndexBuffer;
    class D3D11PipelineStateObject;

    class D3D11Material {
        friend class D3D11SceneGraph;
    public:
        void Append(ID3D11Device& device, x_string const& tag, char const* path);

        [[nodiscard]] x_vector<std::shared_ptr<D3D11ShaderResource>> const& GetShaderResources() const;

        void Bind(ID3D11DeviceContext& context);
    private:
        DirectX::XMFLOAT4 _emissiveColor;
        DirectX::XMFLOAT4 _albedoColor;
        DirectX::XMFLOAT4 _roughness;
        float             _transparencyFactor;
        float             _alphaTest;
        float             _metallicFactor;

        x_vector<std::shared_ptr<D3D11ShaderResource>>  _srs;
    };

    class D3D11Mesh {
        friend class D3D11SceneGraph;
        friend class D3D11SceneMan;
    public:
        D3D11Mesh() = default;
        D3D11Mesh(
            std::shared_ptr<D3D11VertexBuffer>& vertex_buffer,
            std::shared_ptr<D3D11IndexBuffer>& index_buffer,
            D3D11_PRIMITIVE_TOPOLOGY topology,
            D3D11RenderStrategy strategy
        );

        //template <typename RenderStrategy>
        //void Render(ID3D11DeviceContext& context, RenderStrategy&& strategy);
        void Render(ID3D11DeviceContext& context, D3D11Material* material);

        [[nodiscard]] uint32_t GetIndexCount() const;
    private:
        x_string                                      _vertexFormat;
        std::shared_ptr<D3D11VertexBuffer>            _vertexBuffer;
        std::shared_ptr<D3D11IndexBuffer>             _indexBuffer; 
        D3D11_PRIMITIVE_TOPOLOGY                      _topology;
        D3D11RenderStrategy                           _strategy;
    };

    //class D3D11SceneNode {
    //    friend class D3D11SceneGraph;
    //public:
    //    D3D11SceneNode(int32_t id, int32_t parent_id, x_string& name, x_vector<std::shared_ptr<D3D11Mesh>>& meshes, DirectX::XMMATRIX const& transform);

    //    void Render(ID3D11DeviceContext& context);

    //    [[nodiscard]] int32_t GetId() const { return _id; }
    //    [[nodiscard]] x_vector<int32_t> const& GetChildrenIds() const { return _childrenId; }
    //private:
    //    int32_t                                    _id {};
    //    int32_t                                    _parentId {};
    //    x_vector<int32_t>                          _childrenId {};
    //private:
    //    x_string                                   _name {};
    //    // Scene holds all the meshes and each node points to them
    //    x_vector<std::shared_ptr<D3D11Mesh>>       _pMesh {};
    //    DirectX::XMFLOAT4X4                        _transform {};
    //};

    struct D3D11SceneNode {
         int32_t _parent;
         int32_t _firstChild;
         int32_t _firstSibling;
         int32_t _lastSibling;
         int32_t _level;

         int32_t _nextSibling {-1};
    };

    class D3D11SceneGraph {
        friend class D3D11RenderObject;
        friend class D3D11Mesh;

        static constexpr int MAX_NODE_LEVEL {20};
    public:
        D3D11SceneGraph(ID3D11Device& device, char const* path);

        int32_t NumOfNodes() const { return _tree.size(); }

        void MarkAsUpdated(int32_t node);
        void RecalculateGlobalTransforms();
    private:
        int32_t                              ParseNode(int32_t parent_id, int32_t level, aiScene const* ai_scene, aiNode const* ai_node);
        [[nodiscard]] static D3D11Mesh       ParseMesh(ID3D11Device& device, aiMesh const* ai_mesh);
        [[nodiscard]] static D3D11Material   ParseMaterial(ID3D11Device& device, aiMaterial const* ai_material, char const* base_path);
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
        x_vector<DirectX::XMFLOAT4X4>        _globalTransforms;
        x_vector<DirectX::XMFLOAT4X4>        _localTransforms;
    private:
        x_unordered_map<uint32_t, uint32_t>  _nodeId_to_meshId {};
        x_unordered_map<uint32_t, uint32_t>  _nodeId_to_materialId {};
        x_unordered_map<uint32_t, uint32_t>  _nodeId_to_namesId {};
    private:
        // For the Update
        x_vector<int32_t>                    _updated[MAX_NODE_LEVEL] {};
    };

    std::string process_ai_path(char const* base_path, char const* ai_path);
}


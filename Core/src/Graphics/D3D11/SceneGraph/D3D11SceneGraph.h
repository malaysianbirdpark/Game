#pragma once

#include <assimp/Importer.hpp>
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
    public:
        [[nodiscard]] x_vector<std::shared_ptr<D3D11ShaderResource>> const& GetShaderResources() const;
    private:
        x_vector<std::shared_ptr<D3D11ShaderResource>>  _srs;
    };

    class D3D11Mesh {
    public:
        D3D11Mesh() = default;
        D3D11Mesh(
            std::shared_ptr<D3D11VertexBuffer>& vertex_buffer,
            std::shared_ptr<D3D11IndexBuffer>& index_buffer,
            D3D11_PRIMITIVE_TOPOLOGY topology,
            std::unique_ptr<D3D11Material>& material,
            std::unique_ptr<D3D11RenderStrategy>& render_strategy
        );

        //template <typename RenderStrategy>
        //void Render(ID3D11DeviceContext& context, RenderStrategy&& strategy);
        void Render(ID3D11DeviceContext& context);

        [[nodiscard]] uint32_t GetIndexCount() const;
    private:
        x_string                                      _vertexFormat;
        std::shared_ptr<D3D11VertexBuffer>            _vertexBuffer;
        std::shared_ptr<D3D11IndexBuffer>             _indexBuffer; 
        D3D11_PRIMITIVE_TOPOLOGY                      _topology;
        std::unique_ptr<D3D11Material>                _material;
        std::unique_ptr<D3D11RenderStrategy>          _strategy;
    };

    class D3D11SceneNode {
        friend class D3D11SceneGraph;
    public:
        D3D11SceneNode(int32_t id, int32_t parent_id, x_string& name, x_vector<std::shared_ptr<D3D11Mesh>>& meshes, DirectX::XMMATRIX const& transform);

        void Render(ID3D11DeviceContext& context);

        [[nodiscard]] int32_t GetId() const { return _id; }
        [[nodiscard]] x_vector<int32_t> const& GetChildrenIds() const { return _childrenId; }
    private:
        int32_t                                    _id {};
        int32_t                                    _parentId {};
        x_vector<int32_t>                          _childrenId {};
    private:
        x_string                                   _name {};
        // Scene holds all the meshes and each node points to them
        x_vector<std::shared_ptr<D3D11Mesh>>       _pMesh {};
        DirectX::XMFLOAT4X4                        _transform {};
    };

    class D3D11SceneGraph {
    public:
        D3D11SceneGraph(ID3D11Device& device, char const* path, x_vector<x_string> const& vertex_formats_per_mesh);

        void Render(ID3D11DeviceContext& context);

        std::shared_ptr<D3D11SceneNode> GetNodeAt(int32_t id) const { return _sceneTree[id]; }

        int32_t NumOfNodes() const { return _sceneTree.size(); }
        DirectX::XMMATRIX GetTransformAt(int32_t id) const { return DirectX::XMLoadFloat4x4(&_sceneTree[id]->_transform); }
    private:
        [[nodiscard]] std::shared_ptr<D3D11SceneNode>    ParseNode(int32_t id, int32_t parent_id, aiNode const* ai_node) const;
        [[nodiscard]] std::shared_ptr<D3D11Mesh>         ParseMesh(ID3D11Device& device, aiMesh const* ai_mesh, aiMaterial const* ai_material, x_string const& vertex_format) const;
        [[nodiscard]] std::pair<std::unique_ptr<D3D11Material>, std::unique_ptr<D3D11RenderStrategy>>  ParseMaterial(aiMaterial const* ai_material);
        [[nodiscard]] std::pair<std::shared_ptr<D3D11VertexBuffer>, std::shared_ptr<D3D11IndexBuffer>> ParseVertexData(ID3D11Device& device, aiMesh const* ai_mesh, x_string const& vertex_format);
    private:
        x_vector<std::shared_ptr<D3D11Mesh>>           _mesh {};
        x_vector<std::shared_ptr<D3D11SceneNode>>      _sceneTree;
    };
}


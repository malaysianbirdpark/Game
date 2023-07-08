#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine::Graphics {
    class D3D11InputLayout;
    class D3D11VertexBuffer;
    class D3D11IndexBuffer;
    class D3D11PipelineStateObject;
    class D3D11RootSignature;

    namespace Vertex {
        class Layout;        
        class Buffer;
    }

    class D3DMesh {
    public:
        D3DMesh() = default;
        D3DMesh(
            x_string& rs_tags, 
            std::shared_ptr<D3D11VertexBuffer>& vertex_buffer,
            std::shared_ptr<D3D11IndexBuffer>& index_buffer,
            D3D11_PRIMITIVE_TOPOLOGY topology
        );

        void Draw(ID3D11DeviceContext& context, DirectX::FXMMATRIX const& acc_transform);

        DirectX::XMMATRIX GetModelMatrix() const { return DirectX::XMLoadFloat4x4(&_transform); }
    private:
        void SetVertexBuffer(ID3D11Device& device, Vertex::Buffer const& vbuf, char const* tag);
        void SetIndexBuffer(ID3D11Device& device, x_vector<unsigned short> const& indices, char const* tag);
    private:
        x_string                            _rsTag;
        std::shared_ptr<D3D11VertexBuffer>  _vertexBuffer;
        std::shared_ptr<D3D11IndexBuffer>   _indexBuffer;
        D3D11_PRIMITIVE_TOPOLOGY            _topology;
        mutable DirectX::XMFLOAT4X4         _transform;
    };

    class D3DSceneNode {
        friend class D3DScene;
    public:
        D3DSceneNode(x_string& name, x_vector<std::shared_ptr<D3DMesh>>& meshes, DirectX::FXMMATRIX const& parent_transform);

        void Draw(ID3D11DeviceContext& context, DirectX::FXMMATRIX acc_transform);
    private:
        x_string                                 _name {};
        // Scene holds all the meshes and each node points to them
        x_vector<std::shared_ptr<D3DMesh>>       _pMesh {};
        DirectX::XMFLOAT4X4                      _currentTransform {};
        DirectX::XMFLOAT4X4                      _parentTransform {};
        x_vector<std::shared_ptr<D3DSceneNode>>  _child {};
    };

    class D3DScene {
        friend class D3DSceneGraph;
    public:
        D3DScene(DirectX::FXMMATRIX const& parent_transform = DirectX::XMMatrixIdentity());
        D3DScene(ID3D11Device& device, aiScene const* ai_scene, DirectX::FXMMATRIX const& parent_transform = DirectX::XMMatrixIdentity());

        void Draw(ID3D11DeviceContext& context, DirectX::FXMMATRIX acc_transform = DirectX::XMMatrixIdentity());

        void SetMeshes(x_vector<std::shared_ptr<D3DMesh>> const& meshes);
    private:
        [[nodiscard]] std::shared_ptr<D3DSceneNode>  ParseNode(aiNode* const ai_node);
        [[nodiscard]] std::shared_ptr<D3DMesh>       ParseMesh(ID3D11Device& device, aiMesh* const ai_mesh, aiMaterial** const ai_material);
    private:
        x_vector<std::shared_ptr<D3DMesh>>       _mesh {};
        x_vector<std::shared_ptr<D3DScene>>      _childScene {};
        x_vector<std::shared_ptr<D3DSceneNode>>  _sceneTree;
        DirectX::XMFLOAT4X4                      _parentTransform {};
    };

    class D3DSceneGraph {
    public:
        D3DSceneGraph();

        void Update(float const dt);
        void Draw(ID3D11DeviceContext& context);

        void AddScene(ID3D11Device& device, x_string const& file_name);
    private:
        // TODO: root node is currently a dummy node
        // maybe a cubemap could be the root in the future?
        std::shared_ptr<D3DScene> _root;
    };
}


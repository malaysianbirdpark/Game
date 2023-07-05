#include "pch.h"
#include "D3DSceneGraph.h"

#include "D3D11/D3D11VertexBuffer.h"
#include "D3D11/D3D11IndexBuffer.h"

#include "D3D11/PipelineState/D3D11PipelineStateHolder.h"
#include "D3D11/PipelineState/D3D11PSOLibrary.h"

#include "D3D11/RootSignature/D3D11RootSignature.h"
#include "D3D11/RootSignature/D3D11RootSignatureHolder.h"
#include "D3D11/RootSignature/D3D11RSLibrary.h"

#include "D3D11/D3D11MeshDataHolder.h"

#include "D3DVertex.h"

Engine::Graphics::D3DMesh::D3DMesh(x_string& rs_tags, std::shared_ptr<D3D11VertexBuffer>& vertex_buffer, std::shared_ptr<D3D11IndexBuffer>& index_buffer, D3D11_PRIMITIVE_TOPOLOGY topology)
    : _rsTag{std::move(rs_tags)}, _vertexBuffer{std::move(vertex_buffer)}, _indexBuffer{std::move(index_buffer)}, _topology{topology}
{
    GRAPHICS_INFO("Mesh Initialized");
}

void Engine::Graphics::D3DMesh::Draw(ID3D11DeviceContext& context, DirectX::FXMMATRIX const& acc_transform) {
    GRAPHICS_INFO("Draw Mesh");

    DirectX::XMStoreFloat4x4(&_transform, acc_transform);

    // Bind
    context.IASetPrimitiveTopology(_topology);
    _vertexBuffer->Bind(context);
    _indexBuffer->Bind(context);

    // TODO: Multi-thread
    auto const rs {D3D11RSLibrary::ResolveRS(_rsTag)};
    rs->UpdateMVP(context, &acc_transform, sizeof(acc_transform));
    rs->Bind(context);

    context.DrawIndexed(_indexBuffer->GetCount(), 0u, 0u);
}

void Engine::Graphics::D3DMesh::SetVertexBuffer(ID3D11Device& device, Vertex::Buffer const& vbuf, char const* tag) {
    _vertexBuffer = D3D11MeshDataHolder::ResolveVertexBuffer(device, vbuf, tag);
}

void Engine::Graphics::D3DMesh::SetIndexBuffer(ID3D11Device& device, x_vector<unsigned short> const& indices, char const* tag) {
    _indexBuffer = D3D11MeshDataHolder::ResolveIndexBuffer(device, indices, tag);
}

Engine::Graphics::D3DSceneNode::D3DSceneNode(x_vector<std::shared_ptr<D3DMesh>>& meshes, DirectX::FXMMATRIX& parent_transform)
    : _pMesh{std::move(meshes)}
{
    DirectX::XMStoreFloat4x4(&_parentTransform, parent_transform);
    DirectX::XMStoreFloat4x4(&_currentTransform, DirectX::XMMatrixIdentity());
}

Engine::Graphics::D3DScene::D3DScene(DirectX::FXMMATRIX& parent_transform) {
    DirectX::XMStoreFloat4x4(&_parentTransform, parent_transform);
}

Engine::Graphics::D3DScene::D3DScene(ID3D11Device& device, aiScene const* ai_scene, DirectX::FXMMATRIX const& parent_transform) {
    DirectX::XMStoreFloat4x4(&_parentTransform, parent_transform);

    auto const root_node {ai_scene->mRootNode};

    _mesh.reserve(ai_scene->mNumMeshes);
    for (auto i {0}; i != ai_scene->mNumMeshes; ++i)
        _mesh.push_back(std::move(ParseMesh(device, ai_scene->mMeshes[i], ai_scene->mMaterials)));

    _sceneTree.reserve(root_node->mNumChildren);
    for (auto i {0}; i != root_node->mNumChildren; ++i)
        _sceneTree.push_back(std::move(ParseNode(root_node->mChildren[i])));
}

void Engine::Graphics::D3DScene::Draw(ID3D11DeviceContext& context, DirectX::FXMMATRIX acc_transform) {
    auto const transform {
        DirectX::XMLoadFloat4x4(&_parentTransform) *
        acc_transform
    };

    for (auto& m : _mesh)
        m->Draw(context, transform);
    for (auto& ch : _childScene)
        ch->Draw(context, transform);
}

void Engine::Graphics::D3DScene::SetMeshes(x_vector<std::shared_ptr<D3DMesh>> const& meshes) {
    _mesh = std::move(meshes);
}

std::shared_ptr<Engine::Graphics::D3DSceneNode> Engine::Graphics::D3DScene::ParseNode(aiNode* const ai_node) {
    auto const transform {
        DirectX::XMMatrixTranspose(
            DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 const*>(&ai_node->mTransformation))
        )
    };

    x_vector<std::shared_ptr<D3DMesh>> temp;
    for (auto i {0}; i != ai_node->mNumMeshes; ++i)
        temp.push_back(_mesh[ai_node->mMeshes[i]]);

    auto new_node {MakeShared<D3DSceneNode>(temp, transform)};
    for (auto i {0}; i != ai_node->mNumChildren; ++i)
        new_node->_child.push_back(std::move(ParseNode(ai_node->mChildren[i])));

    return new_node;
}

std::shared_ptr<Engine::Graphics::D3DMesh> Engine::Graphics::D3DScene::ParseMesh(ID3D11Device& device, aiMesh* const ai_mesh, aiMaterial** const ai_material) {
    Vertex::Buffer vbuf {
        std::move(
            Vertex::Layout{}.Append(Vertex::Layout::Position3D)
                            .Append(Vertex::Layout::Normal)
                            .Append(Vertex::Layout::Texture2D)
        )
    };

    for (auto i {0}; i != ai_mesh->mNumVertices; ++i)
        vbuf.EmplaceBack(
            *reinterpret_cast<DirectX::XMFLOAT3*>(&ai_mesh->mVertices[i]),
            *reinterpret_cast<DirectX::XMFLOAT3*>(&ai_mesh->mNormals[i]),
            *reinterpret_cast<DirectX::XMFLOAT2*>(&ai_mesh->mTextureCoords[0][i])
        );

    x_vector<uint16_t> indices;
    indices.reserve(ai_mesh->mNumFaces * 3);
    for (auto i {0}; i != ai_mesh->mNumFaces; ++i) {
        auto const& face {ai_mesh->mFaces[i]};
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    auto vertex_buffer {D3D11MeshDataHolder::ResolveVertexBuffer(device, vbuf, "air_balloon")};
    auto index_buffer {D3D11MeshDataHolder::ResolveIndexBuffer(device, indices, "air_balloon")};

    x_string tag {"basic"};
    return MakeShared<D3DMesh>(tag, vertex_buffer, index_buffer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

Engine::Graphics::D3DSceneGraph::D3DSceneGraph() {
    _root = MakeShared<D3DScene>();
}

void Engine::Graphics::D3DSceneGraph::Update(float const dt) {
}

void Engine::Graphics::D3DSceneGraph::Draw(ID3D11DeviceContext& context) {
    _root->Draw(context);
}

void Engine::Graphics::D3DSceneGraph::AddScene(ID3D11Device& device, x_string const& file_name) {
    Assimp::Importer imp;
    auto const scene {imp.ReadFile(
        file_name.c_str(),
        aiProcess_ConvertToLeftHanded |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenNormals
    )};

    _root->_childScene.push_back(std::move(MakeShared<D3DScene>(device, scene)));
}

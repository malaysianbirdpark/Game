#include "pch.h"
#include "D3D11SceneGraph.h"

#include "D3D11VertexBuffer.h"
#include "D3D11IndexBuffer.h"

#include "D3D11ModelQuery.h"

#include "Graphics/D3D11/ShaderResource/D3D11ShaderResourcesInclude.h"
#include "Graphics/D3D11/RenderStrategy/D3D11RenderStrategyInclude.h"

#include <variant>

Engine::Graphics::D3D11Mesh::D3D11Mesh(std::shared_ptr<D3D11VertexBuffer>& vertex_buffer, std::shared_ptr<D3D11IndexBuffer>& index_buffer, x_vector<std::shared_ptr<D3D11ShaderResource>>& shader_resources, D3D11_PRIMITIVE_TOPOLOGY topology)
    : _vertexBuffer{std::move(vertex_buffer)}, _indexBuffer{std::move(index_buffer)}, _srs{std::move(shader_resources)}, _topology{topology}
{
    GRAPHICS_INFO("Mesh Initialized");
}

// Use this version when you want to render this mesh using other strategy
//template <typename RenderStrategy>
//void Engine::Graphics::D3D11Mesh::Render(ID3D11DeviceContext& context, RenderStrategy&& strategy) {
//    context.IASetPrimitiveTopology(_topology);
//    _vertexBuffer->Bind(context);
//    _indexBuffer->Bind(context);
//
//    strategy->Render(context, _indexBuffer->GetCount(), *this);
//}

void Engine::Graphics::D3D11Mesh::Render(ID3D11DeviceContext& context) {
    // Bind
    context.IASetPrimitiveTopology(_topology);
    _vertexBuffer->Bind(context);
    _indexBuffer->Bind(context);

    std::visit(StrategicRender{context, this}, *_strategy);
}

Engine::x_vector<std::shared_ptr<Engine::Graphics::D3D11ShaderResource>> const& Engine::Graphics::D3D11Mesh::GetShaderResources() const {
    return _srs;
}

uint32_t Engine::Graphics::D3D11Mesh::GetIndexCount() const {
    return _indexBuffer->GetCount();
}

Engine::Graphics::D3D11SceneNode::D3D11SceneNode(int32_t id, int32_t parent_id, x_string& name, x_vector<std::shared_ptr<D3D11Mesh>>& meshes, DirectX::XMMATRIX const& transform)
    : _id{id}, _parentId{parent_id}, _name{std::move(name)}, _pMesh{std::move(meshes)}
{
    DirectX::XMStoreFloat4x4(&_transform, transform);
}

void Engine::Graphics::D3D11SceneNode::Render(ID3D11DeviceContext& context) {
    for (auto const& m : _pMesh)
        m->Render(context);
}

Engine::Graphics::D3D11SceneGraph::D3D11SceneGraph(ID3D11Device& device, char const* path) {
    Assimp::Importer imp;
    auto const ai_scene {imp.ReadFile(
        path,
        aiProcess_ConvertToLeftHanded |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenNormals
    )};

    auto const root_node {ai_scene->mRootNode};

    _mesh.reserve(ai_scene->mNumMeshes);
    for (auto i {0}; i != ai_scene->mNumMeshes; ++i)
        _mesh.push_back(std::move(ParseMesh(device, ai_scene->mMeshes[i], ai_scene->mMaterials)));

    // For the Root Node
    _sceneTree.reserve(root_node->mNumChildren);

    std::queue<std::pair<int32_t, aiNode*>> bfs_queue;
    uint32_t id {0};

    for (auto i {0}; i != root_node->mNumChildren; ++i)
        _sceneTree.push_back(std::move(ParseNode(++id, 0, root_node->mChildren[i])));
    for (auto i {0}; i != root_node->mNumChildren; ++i)
        bfs_queue.emplace(0, root_node);

    // For the children
    while (!bfs_queue.empty()) {
        auto const [parent_id, current_node] {bfs_queue.front()};
        bfs_queue.pop();

        _sceneTree.push_back(std::move(ParseNode(++id, parent_id, current_node)));

        for (auto i {0}; i != current_node->mNumChildren; ++i)
            bfs_queue.emplace(parent_id, current_node->mChildren[i]);
    }
}

void Engine::Graphics::D3D11SceneGraph::Render(ID3D11DeviceContext& context) {
    for (auto const& node : _sceneTree)
        node->Render(context);
}

std::shared_ptr<Engine::Graphics::D3D11SceneNode> Engine::Graphics::D3D11SceneGraph::ParseNode(int32_t id, int32_t parent_id, aiNode const* ai_node) const {
    auto const transform {
        DirectX::XMMatrixTranspose(
            DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 const*>(&ai_node->mTransformation))
        )
    };

    x_vector<std::shared_ptr<D3D11Mesh>> temp;
    for (auto i {0}; i != ai_node->mNumMeshes; ++i)
        temp.push_back(_mesh[ai_node->mMeshes[i]]);

    x_string name {ai_node->mName.C_Str()};
    return MakeShared<D3D11SceneNode>(id, parent_id, name, temp, transform);
}

std::shared_ptr<Engine::Graphics::D3D11Mesh> Engine::Graphics::D3D11SceneGraph::ParseMesh(ID3D11Device& device, aiMesh const* ai_mesh, aiMaterial const* const* ai_material) const {
    D3D11ModelQuery query {};

    auto [tag, vertex_buffer, index_buffer] {query.Cook(device, ai_mesh, ai_material[ai_mesh->mMaterialIndex])};

    return MakeShared<D3D11Mesh>(tag, vertex_buffer, index_buffer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

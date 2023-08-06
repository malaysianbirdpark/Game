#include "pch.h"
#include "D3D11RenderObject.h"

#include "ConstantBuffer/D3D11TransformMVP.h"
#include "SceneGraph/D3D11SceneGraph.h"

Engine::Graphics::D3D11RenderObject::D3D11RenderObject(ID3D11Device& device, DirectX::XMMATRIX const& view, DirectX::XMMATRIX const& proj, DirectX::XMMATRIX const& root_transform, std::shared_ptr<D3D11SceneGraph>&& scene)
    : _scene{std::move(scene)}
{
    DirectX::XMStoreFloat4x4(&_rootTransform, root_transform);

    _transformOfMeshes.reserve(_scene->NumOfNodes());
    for (auto i {0}; i != _scene->NumOfNodes(); ++i)
        _transformOfMeshes.push_back(std::move(MakeUnique<D3D11TransformMVP>(device, _scene->GetTransformAt(i), view, proj)));
}

void Engine::Graphics::D3D11RenderObject::Update(ID3D11DeviceContext& context, DirectX::FXMMATRIX& transform, float const dt) {
    auto const root_transform {
        DirectX::XMMatrixTranspose(
            DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&_rootTransform), transform) * dt
        )
    };

    // Perform DFS to update all the MVP of each nodes
    std::stack<std::pair<DirectX::XMMATRIX, std::shared_ptr<D3D11SceneNode>>> dfs_stack {};
    dfs_stack.emplace(root_transform, _scene->GetNodeAt(0));

    while (!dfs_stack.empty()) {
        auto const [parent_transform, current_node] {dfs_stack.top()};
        dfs_stack.pop();

        auto const current_id {current_node->GetId()};

        auto const current_transform {
            DirectX::XMMatrixTranspose(
                DirectX::XMMatrixMultiply(_scene->GetTransformAt(current_id), parent_transform)
            )
        };

        _transformOfMeshes[current_id]->SetModel(current_transform);
        _transformOfMeshes[current_id]->Update(context);

        for (auto const child_id : current_node->GetChildrenIds()) {
            dfs_stack.emplace(
                current_transform,
                _scene->GetNodeAt(child_id)
            );
        }
    }

}

void Engine::Graphics::D3D11RenderObject::Render(ID3D11DeviceContext& context) {
    _scene->Render(context);
}
